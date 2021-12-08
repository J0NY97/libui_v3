#include "libui.h"

void	ui_layout_event(t_ui_layout *layout, SDL_Event e)
{
	t_list			*curr;
	t_ui_window		*win;
	t_ui_element	*elem;

	curr = layout->windows;
	while (curr)
	{
		win = curr->content;
		if (!win->user_handled_event)
			ui_window_event(win, e);
		curr = curr->next;
	}
	ui_list_event(layout->elements, e);
}

void	ui_layout_render(t_ui_layout *layout)
{
	t_list			*curr;
	t_ui_window		*win;

	ui_list_sort(layout->elements);
	ui_layout_list_render(layout->elements);
	curr = layout->windows;
	while (curr)
	{
		win = curr->content;
		ui_window_render(win);
		curr = curr->next;
	}
}

void	ui_layout_load(t_ui_layout *layout, char *file)
{
	memset(layout, 0, sizeof(t_ui_layout));

	layout_read_file(layout, file);
	if (layout->layout_file_content == NULL)
		return ;
	layout_split_elements(layout);
	layout_make_family_trees(layout);
	layout_compile_elements(layout);
	if (!layout->style_file)
	{
		ft_printf("[%s] We have no style file given, so lets just then dont do anything.\n", __FUNCTION__);
		return ;
	}
	layout_read_style(layout);
	if (layout->style_file_content == NULL)
		return ;
	layout_split_styles(layout);
	layout_make_recipes(layout);
	layout_apply_style(layout);
}

int	get_special(t_ui_layout *layout, char *str)
{
	char	**arr;
	int		result;

	result = 0;
	arr = ft_strsplitfirstoccurence(str, ':');
	if (!arr || !arr[0] || !arr[1])
	{
		ft_arraydel(arr);
		return (0);
	}
	ft_strtrimwholearr(arr);
	if (ft_strequ(arr[0], "style"))
	{
		layout->style_file = ft_strndup(arr[1], ft_strlen(arr[1]) - 1); // remove ';' from the end;
		result = 1;
	}
	else if (ft_strequ(arr[0], "resource_dir"))
	{
		char	*temp;
		temp = ft_strndup(arr[1], ft_strlen(arr[1]) - 1);
		layout->resource_dirs = ft_arradd(layout->resource_dirs, temp);
		ft_strdel(&temp);
		result = 1;
	}
	ft_arraydel(arr);
	return (result);
}

char	*get_file_content(t_ui_layout *layout, char *file)
{
	char	*content;
	char	*line;
	char	*trim;
	size_t	len;
	FILE	*fd;

	fd = fopen(file, "r");
	if (!fd)
	{
		ft_printf("[%s] Couldn\'t open file <%s>\n", __FUNCTION__, file);
		return (NULL);
	}
	line = NULL;
	content = NULL;
	while (1)
	{
		if (getline(&line, &len, fd) == -1)
			break ;
		trim = ft_supertrim(line);
		if (trim != NULL && !get_special(layout, trim)) // whole line was trimmed if is NULL (could happen if the line was a comment);
			ft_stradd(&content, trim);
		ft_strdel(&trim);
	}
	ft_strdel(&line);
	fclose(fd);
	return (content);
}

void	layout_read_file(t_ui_layout *layout, char *file)
{
	layout->layout_file = ft_strdup(file);
	layout->layout_file_content = get_file_content(layout, file);
	ft_printf("[%s] %s : \nContent: %s\n", __FUNCTION__, layout->layout_file, layout->layout_file_content);
}

char	**split_string_into_array(char *str)
{
	char	**final;
	int	i;
	int	j;
	int	opening;
	int	closing;
	int	prev_i;
	int	elem_count;

	i = -1;
	j = -1;
	opening = 0;
	closing = 0;
	prev_i = 0;
	elem_count = 0;
	final = malloc(sizeof(char *) * 1);
	while (str[++i])
	{
		if (str[i] == ';' && opening == closing)
		{
			final = realloc(final, sizeof(char *) * ++elem_count);
			final[elem_count - 1] = ft_strsub(str, prev_i, i - prev_i + 1);
			prev_i = i + 1;
		}
		else if (str[i] == '{')
			opening++;
		else if (str[i] == '}')
			closing++;
	}
	final = realloc(final, sizeof(char *) * ++elem_count);
	final[elem_count - 1] = 0;
	return (final);
}

/*
 * This splits everything from global scope separated with ';', to separeate strings;
*/
void	layout_split_elements(t_ui_layout *layout)
{
	int	k;

	layout->layout_element_strings = split_string_into_array(layout->layout_file_content);
	ft_printf("[%s] Done splitting elems.\n", __FUNCTION__);
	k = -1;
	while (layout->layout_element_strings[++k])
		ft_printf("#%d : \n%s\n", k, layout->layout_element_strings[k]);
}

/*
 * Returns a list of families;
*/
t_ui_family	*make_family_from_string(char *str)
{
	t_list		*families;
	t_ui_family	*family;
	t_ui_family	*child;
	char		**temp;
	char		**type_n_name;
	int			len;
	int			rm_amount;
	char		*children;

	ft_printf("[%s] Starting making families.\n", __FUNCTION__);
	temp = ft_strsplitfirstoccurenceor(str, '{', ';');
	len = ft_strlen(temp[1]);
	children = NULL;
	if (len == 0) // probably some special stuff (like style : "path")
	{
		ft_arraydel(temp);
		return (NULL);
	}
	family = ft_memalloc(sizeof(t_ui_family));
	rm_amount = 0;
	if (temp[1][len - 1] == ';') // remove '}' and/or ';';
	{
		rm_amount++;
		if (temp[1][len - 2] == '}')
			rm_amount++;
		children = ft_strndup(temp[1], len - rm_amount);
		ft_printf("%s\n", children);
	}
	else
		ft_printf("[%s] We didnt make children.\n", __FUNCTION__);
	type_n_name = ft_strsplit(temp[0], ' ');
	ft_putarr(type_n_name);
	ft_printf("%s %s\n{\n%s\n}\n", type_n_name[0], type_n_name[1], children);
	family->parent_id = ft_strdup(type_n_name[1]);
	family->parent_type = ui_element_type_from_string(type_n_name[0]);
	family->children_strings = split_string_into_array(children);
	ft_strdel(&children);
	int k = -1;
	while (family->children_strings[++k])
	{
		child = make_family_from_string(family->children_strings[k]);
		if (!child)
			break ;
		add_to_list(&family->children, child, sizeof(t_ui_family));
	}
	ft_arraydel(temp);
	ft_arraydel(type_n_name);
	ft_printf("[%s] Family made.\n", __FUNCTION__);
	return (family);
}

void	print_family(t_ui_family *family, int nth)
{
	t_list	*curr;
	int tabs;

	tabs = -1;
	while (++tabs < nth)
		ft_putchar('\t');
	ft_printf("%s\n", family->parent_id);
	tabs = -1;
	while (++tabs < nth)
		ft_putchar('\t');
	ft_putstr("{\n");
	curr = family->children;
	while (curr)
	{
		print_family(curr->content, nth + 1);
		curr = curr->next;
	}
	tabs = -1;
	while (++tabs < nth)
		ft_putchar('\t');
	ft_putstr("}\n");
}

/*
 * Takes the layout_file_content and makes the elements with their children.
 * This find window and creates its elements.
*/
void	layout_make_family_trees(t_ui_layout *layout)
{
	int			i;
	char		**arr;
	t_ui_family	*family;

	arr = layout->layout_element_strings;
	if (!arr)
		return ;
	i = -1;
	while (arr[++i])
	{
		family = make_family_from_string(arr[i]);
		if (family)
			add_to_list(&layout->families, family, sizeof(t_ui_family));
	}
	// printing to make sure that the family is setup correctly;
	ft_printf("[%s] Print Families\n", __FUNCTION__);
	print_family(layout->families->content, 0);
}

void	make_elements_from_family(t_list **list, t_ui_window *win, void *parent, int parent_type, t_ui_family *family)
{
	t_ui_element	*elem;
	t_list			*rruc;

	// if the parent element is of type that has another element contained in it. (button has label, dropdown has menu...)
	// make that child element the id of whatever you have decided it to be. no need to make new element since we already
	// have that.
	elem = NULL;
	// If the element type has a getter, we try to get the child,
	// but if the element doesnt have of that type of child,
	// we make a new elem.
	if (parent_type == UI_TYPE_ELEMENT
		&& g_acceptable[((t_ui_element *)parent)->element_type].getter)
	{
		elem = g_acceptable[((t_ui_element *)parent)->element_type].getter(parent, family->parent_type);
		if (elem)
			ui_element_set_id(elem, family->parent_id);
	}
	if (!elem)
	{
		elem = ft_memalloc(sizeof(t_ui_element));
		g_acceptable[family->parent_type].maker(win, elem);
		ui_element_set_parent(elem, parent, parent_type);
		ui_element_set_id(elem, family->parent_id);
	}
	rruc = family->children;
	while (rruc)
	{
		make_elements_from_family(list, win, elem, UI_TYPE_ELEMENT, rruc->content);
		rruc = rruc->next;
	}
	add_to_list(list, elem, UI_TYPE_ELEMENT); // this should be done here so that in the end, the parent will be rendered before children;
}

/*
 * This goes through family and searches for window as the head parent,
 * since all elements need a window, to be created.
*/
void	layout_compile_elements(t_ui_layout *layout)
{
	t_list		*curr;
	t_list		*rruc;
	t_ui_family	*family;
	t_ui_window	*win;

	curr = layout->families;
	while (curr)
	{
		family = curr->content;
		if (family->parent_type == UI_TYPE_WINDOW)
		{
			ft_printf("[%s] Creating window : %s\n", __FUNCTION__, family->parent_id);
			win = ft_memalloc(sizeof(t_ui_window));
			ui_window_new(win, NULL, vec4(0, 25, 100, 100));
			win->layout = layout;
			ui_window_id_set(win, family->parent_id);
			add_to_list(&layout->windows, win, UI_TYPE_WINDOW);
			rruc = family->children;
			while (rruc)
			{
				make_elements_from_family(&layout->elements, win, win, UI_TYPE_WINDOW, rruc->content);
				rruc = rruc->next;
			}
		}
		curr = curr->next;
	}
	//print_all_elements_in_list(layout->elements);
}

void	layout_read_style(t_ui_layout *layout)
{
	ft_printf("[%s] Lets see if you have some style.\n", __FUNCTION__);
	layout->style_file_content = get_file_content(layout, layout->style_file);//get_style_content(layout->style_file);
	ft_printf("%s\n", layout->style_file_content);
}

void	layout_split_styles(t_ui_layout *layout)
{
	int	k;

	layout->style_recipe_strings = split_string_into_array(layout->style_file_content);
	ft_printf("[%s] Done splitting recipes.\n", __FUNCTION__);
	k = -1;
	while (layout->style_recipe_strings[++k])
		ft_printf("#%d : \n%s\n", k, layout->style_recipe_strings[k]);
}

char	**ft_arrjoin(char **to_this, char **from_this)
{
	char	**arr;
	int		i;
	int		j;

	i = 0;
	j = 0;
	arr = NULL;
	if (!to_this && !from_this)
		return (NULL);
	while (to_this && to_this[i])
	{
		arr = realloc(arr, sizeof(char *) * (i + 1));
		arr[i] = ft_strdup(to_this[i]);
		i++;
	}
	while (from_this && from_this[j])
	{
		arr = realloc(arr, sizeof(char *) * (i + j + 1));
		arr[i + j] = ft_strdup(from_this[j]);
		j++;
	}
	arr = realloc(arr, sizeof(char *) * (i + j + 1));
	arr[i + j] = 0;
	return (arr);
}

/*
 * both target and child should always be !NULL;
*/
void	fill_recipe_from_recipe(t_ui_recipe *target, t_ui_recipe *child)
{
	int	jj;

	jj = -1;
	while (++jj < VEC4_SIZE)
	{
		if (child->pos_set[jj])
		{
			target->pos.v[jj] = child->pos.v[jj];
			target->pos_set[jj] = 1;
		}
	}
	jj = -1;
	while (++jj < UI_STATE_AMOUNT)
	{
		if (child->bg_colors_set[jj])
		{
			target->bg_colors[jj] = child->bg_colors[jj];
			target->bg_colors_set[jj] = 1;
		}
	}
	jj = -1;
	while (++jj < UI_STATE_AMOUNT)
	{
		if (child->bg_images_set[jj])
		{
			if (target->bg_images_set[jj])
				ft_strdel(&target->bg_images[jj]);
			target->bg_images[jj] = ft_strdup(child->bg_images[jj]);
			target->bg_images_set[jj] = 1;
		}
	}
	if (child->z_set)
	{
		target->z = child->z;
		target->z_set = 1;
	}
	if (child->show_set)
	{
		target->show = child->show;
		target->show_set = 1;
	}
	if (child->title != NULL)
	{
		if (target->title)
			ft_strdel(&target->title);
		target->title = ft_strdup(child->title);
	}
	if (child->remove_title)
		target->remove_title = 1;
	if (child->placeholder_text != NULL)
	{
		if (target->placeholder_text)
			ft_strdel(&target->placeholder_text);
		target->placeholder_text = ft_strdup(child->placeholder_text);
	}
	if (child->text_color_set)
	{
		target->text_color = child->text_color;
		target->text_color_set = 1;
	}
	if (child->text_size_set)
	{
		target->text_size = child->text_size;
		target->text_size_set = 1;
	}
	if (child->font != NULL)
	{
		if (target->font)
			ft_strdel(&target->font);
		target->font = ft_strdup(child->font);
	}
	if (child->text_align_set)
	{
		target->text_align = child->text_align;
		target->text_align_set = 1;
	}
	jj = -1;
	while (++jj < 3)
	{
		if (child->value_set[jj])
		{
			target->value[jj] = child->value[jj];
			target->value_set[jj] = 1;
		}
	}
	if (child->flags != NULL) // TODO: this is definitely not correct, i think we should add the flags not replace;
	{
		char	**flags = ft_arrjoin(target->flags, child->flags);
		ft_arraydel(target->flags);
		target->flags = flags;
	}
	if (child->input_type_set)
	{
		target->input_type = child->input_type;
		target->input_type_set = 1;
	}
	if (child->target)
		target->target = ft_strdup(child->target);
	jj = -1;
	while (++jj < 4)
	{
		if (child->text_pos_set[jj])
		{
			target->text_pos.v[jj] = child->text_pos.v[jj];
			target->text_pos_set[jj] = 1;
		}
	}
}

void	fill_recipe_from_args(t_ui_recipe *recipe, char **args)
{
	int		i;
	char	**key_value;
	int		jj;


	if (!args)
		return ;
	i = -1;
	while (args[++i])
	{
		key_value = ft_strsplitfirstoccurence(args[i], ':');
		ft_strtrimwholearr(key_value);
		if (ft_strequ(key_value[0], "x"))
		{
			recipe->pos.v[0] = ft_atof(key_value[1]);
			recipe->pos_set[0] = 1;
		}
		else if (ft_strequ(key_value[0], "y"))
		{
			recipe->pos.v[1] = ft_atof(key_value[1]);
			recipe->pos_set[1] = 1;
		}
		else if (ft_strequ(key_value[0], "w"))
		{
			recipe->pos.v[2] = ft_atof(key_value[1]);
			recipe->pos_set[2] = 1;
		}
		else if (ft_strequ(key_value[0], "h"))
		{
			recipe->pos.v[3] = ft_atof(key_value[1]);
			recipe->pos_set[3] = 1;
		}
		else if (ft_strequ(key_value[0], "xywh"))
		{
			char	**pos;
			int		pp;

			pp = -1;
			pos = ft_strsplit(key_value[1], ',');
			while (++pp < VEC4_SIZE)
			{
				recipe->pos.v[pp] = ft_atof(pos[pp]);
				recipe->pos_set[pp] = 1;
			}
			ft_arraydel(pos);
		}
		else if (ft_strequ(key_value[0], "text_pos"))
		{
			char	**pos;
			int		pp;

			pp = -1;
			pos = ft_strsplit(key_value[1], ',');
			while (++pp < VEC4_SIZE)
			{
				recipe->text_pos.v[pp] = ft_atof(pos[pp]);
				recipe->text_pos_set[pp] = 1;
			}
			ft_arraydel(pos);
		}
		else if (ft_strequ(key_value[0], "bg_color"))
		{
			jj = -1;
			while (++jj < UI_STATE_AMOUNT)
			{
				recipe->bg_colors[jj] = (unsigned int)strtoul(key_value[1], NULL, 16);
				recipe->bg_colors_set[jj] = 1;
			}
		}
		else if (ft_strequ(key_value[0], "bg_color_default"))
		{
			recipe->bg_colors[UI_STATE_DEFAULT] = (unsigned int)strtoul(key_value[1], NULL, 16);
			recipe->bg_colors_set[UI_STATE_DEFAULT] = 1;
		}
		else if (ft_strequ(key_value[0], "bg_color_hover"))
		{
			recipe->bg_colors[UI_STATE_HOVER] = (unsigned int)strtoul(key_value[1], NULL, 16);
			recipe->bg_colors_set[UI_STATE_HOVER] = 1;
		}
		else if (ft_strequ(key_value[0], "bg_color_click"))
		{
			recipe->bg_colors[UI_STATE_CLICK] = (unsigned int)strtoul(key_value[1], NULL, 16);
			recipe->bg_colors_set[UI_STATE_CLICK] = 1;
		}
		else if (ft_strequ(key_value[0], "bg_image"))
		{
			jj = -1;
			while (++jj < UI_STATE_AMOUNT)
			{
				if (recipe->bg_images_set[jj])
					ft_strdel(&recipe->bg_images[jj]);
				recipe->bg_images[jj] = ft_strdup(key_value[1]);
				recipe->bg_images_set[jj] = 1;
			}
		}
		else if (ft_strequ(key_value[0], "bg_image_default"))
		{
			if (recipe->bg_images_set[UI_STATE_DEFAULT])
				ft_strdel(&recipe->bg_images[UI_STATE_DEFAULT]);
			recipe->bg_images[UI_STATE_DEFAULT] = ft_strdup(key_value[1]);
			recipe->bg_images_set[UI_STATE_DEFAULT] = 1;
		}
		else if (ft_strequ(key_value[0], "bg_image_hover"))
		{
			if (recipe->bg_images_set[UI_STATE_HOVER])
				ft_strdel(&recipe->bg_images[UI_STATE_HOVER]);
			recipe->bg_images[UI_STATE_HOVER] = ft_strdup(key_value[1]);
			recipe->bg_images_set[UI_STATE_HOVER] = 1;
		}
		else if (ft_strequ(key_value[0], "bg_image_click"))
		{
			if (recipe->bg_images_set[UI_STATE_CLICK])
				ft_strdel(&recipe->bg_images[UI_STATE_CLICK]);
			recipe->bg_images[UI_STATE_CLICK] = ft_strdup(key_value[1]);
			recipe->bg_images_set[UI_STATE_CLICK] = 1;
		}
		else if (ft_strequ(key_value[0], "z"))
		{
			recipe->z = ft_atoi(key_value[1]);
			recipe->z_set = 1;
		}
		else if (ft_strequ(key_value[0], "show"))
		{
			recipe->show = ft_atoi(key_value[1]);
			recipe->show_set = 1;
		}
		else if (ft_strequ(key_value[0], "title"))
		{
			if (recipe->title)
				ft_strdel(&recipe->title);
			if (!key_value[1])
				recipe->remove_title = 1;
			else
				recipe->title = ft_strdup(key_value[1]);
		}
		else if (ft_strequ(key_value[0], "text_color"))
		{
			recipe->text_color = (unsigned int)strtoul(key_value[1], NULL, 16);
			recipe->text_color_set = 1;
		}
		else if (ft_strequ(key_value[0], "text_size"))
		{
			recipe->text_size = ft_atoi(key_value[1]); // should we check if this number is something that makes sense;
			recipe->text_size_set = 1;
		}
		else if (ft_strequ(key_value[0], "font"))
		{
			if (recipe->font)
				ft_strdel(&recipe->font);
			recipe->font = ft_strdup(key_value[1]);
		}
		else if (ft_strequ(key_value[0], "text_align"))
		{
			recipe->text_align = text_align_getter(key_value[1]);
			recipe->text_align_set = 1;
		}
		else if (ft_strequ(key_value[0], "value") || ft_strequ(key_value[0], "values"))
		{
			char	**values;

			jj = -1;
			values = ft_strsplit(key_value[1], ',');
			while (++jj < 3)
			{
				recipe->value[jj] = ft_atoi(values[jj]);
				recipe->value_set[jj] = 1;
			}
			ft_arraydel(values);
		}
		else if (ft_strequ(key_value[0], "flag") || ft_strequ(key_value[0], "flags"))
		{
			char	**flags = ft_strsplit(key_value[1], ' ');
			char	**final_flags = ft_arrjoin(recipe->flags, flags);
			ft_arraydel(flags);
			ft_arraydel(recipe->flags);
			recipe->flags = final_flags;
		}
		else if (ft_strequ(key_value[0], "type"))
		{
			if (ft_strequ(key_value[1], "numbers"))
			{
				recipe->input_type = 1;
				recipe->input_type_set = 1;
			}
			else if (ft_strequ(key_value[1], "letters"))
			{
				recipe->input_type = 2;
				recipe->input_type_set = 1;
			}
			else if (ft_strequ(key_value[1], "everything"))
			{
				recipe->input_type = 0;
				recipe->input_type_set = 1;
			}
		}
		else if (ft_strequ(key_value[0], "target"))
		{
			recipe->target = ft_strdup(key_value[1]);
		}
		else if (ft_strequ(key_value[0], "placeholder"))
		{
			if (recipe->placeholder_text)
				ft_strdel(&recipe->placeholder_text);
			recipe->placeholder_text = ft_strdup(key_value[1]);
		}
		ft_arraydel(key_value);
	}
}

t_ui_recipe	*make_recipe_from_string(t_list *windows, t_list *elements, t_list *recipes, char *str)
{
	t_ui_element	*elem;
	t_ui_window		*win;
	t_ui_recipe	*recipe;
	t_ui_recipe	*child_recipe;
	char			**temp;
	char			**name_and_prefabs;
	char			**args;
	int				i;

	if (!str)
		return (NULL);
	temp = ft_strsplitfirstoccurence(str, '{');
	name_and_prefabs = ft_strsplit(temp[0], ':');
	ft_strtrimwholearr(name_and_prefabs);
	elem = ui_list_get_element_by_id(elements, name_and_prefabs[0]);
	win = ui_list_get_window_by_id(windows, name_and_prefabs[0]);
	args = ft_strsplit(temp[1], ';');
	ft_strtrimwholearr(args);
	ft_arraydel(temp);
	ft_putarr(name_and_prefabs);
	ft_putarr(args);
	recipe = ft_memalloc(sizeof(t_ui_recipe));
	recipe->id = ft_strdup(name_and_prefabs[0]);
	ft_printf("[%s] Making recipe : %s\n", __FUNCTION__, recipe->id);
	if (elem || win) // if we have found elem with the same id;
	{
		if (elem)
			child_recipe = ui_list_get_recipe_by_id(recipes, g_acceptable[elem->element_type].name);
		else if (win)
			child_recipe = ui_list_get_recipe_by_id(recipes, g_acceptable[UI_TYPE_WINDOW].name);
		if (child_recipe)
		{
			ft_printf("child_recipe with id : %s\n", child_recipe->id);
			fill_recipe_from_recipe(recipe, child_recipe);
			ft_putstr("recipe was filled by child : \n");
			ft_putarr(recipe->flags);
		}
		else
			ft_printf("[%s] No 'global' recipe for that type.\n", __FUNCTION__);
	}
	else
		ft_printf("[%s] No element nor window with recipe id %s found.\n", __FUNCTION__, name_and_prefabs[0]);
	i = 0; // start at 0 because id is at 0, so we dont want to check that;
	while (name_and_prefabs[++i])
	{
		child_recipe = ui_list_get_recipe_by_id(recipes, name_and_prefabs[i]);
		if (!child_recipe)
			ft_printf("[%s] No recipe with id %s found, couldn\'t copy recipe.\n", __FUNCTION__, name_and_prefabs[i]);
		else
			fill_recipe_from_recipe(recipe, child_recipe);
	}
	fill_recipe_from_args(recipe, args);
	ft_putstr("final : \n");
	ft_putarr(recipe->flags);
	ft_arraydel(name_and_prefabs);
	ft_arraydel(args);
	return (recipe);
}

void	ui_recipe_print(t_ui_recipe *recipe)
{
	ft_printf("%s\n", recipe->id);
	ft_printf("pos_set : %d %d %d %d\n",
		recipe->pos_set[0], recipe->pos_set[1],
		recipe->pos_set[2], recipe->pos_set[3]);
	if (recipe->pos_set[0] || recipe->pos_set[1]
		|| recipe->pos_set[2] || recipe->pos_set[3])
	{
		ft_printf("pos : ");
		print_vec(recipe->pos.v, 4);
	}
	if (recipe->title)
		ft_printf("title : %s\n", recipe->title);
	if (recipe->text_color_set)
		ft_printf("text_color : %#x\n", recipe->text_color);
	if (recipe->text_align_set)
		ft_printf("text_align : [%d] %s\n", recipe->text_align, text_align_to_str(recipe->text_align));
	if (recipe->text_pos_set[0] || recipe->text_pos_set[1]
		|| recipe->text_pos_set[2] || recipe->text_pos_set[3])
	{
		ft_printf("text_pos : ");
		print_vec(recipe->text_pos.v, 4);
	}
	if (recipe->font)
		ft_printf("font : %s\n", recipe->font);
	if (recipe->bg_colors_set[0])
		ft_printf("bg_color_default : %#x\n", recipe->bg_colors[UI_STATE_DEFAULT]);
	if (recipe->bg_colors_set[1])
		ft_printf("bg_color_hover : %#x\n", recipe->bg_colors[UI_STATE_HOVER]);
	if (recipe->bg_colors_set[2])
		ft_printf("bg_color_click : %#x\n", recipe->bg_colors[UI_STATE_CLICK]);
	if (recipe->bg_images_set[0])
		ft_printf("bg_image_default : %s\n", recipe->bg_images[UI_STATE_DEFAULT]);
	if (recipe->bg_images_set[1])
		ft_printf("bg_image_hover : %s\n", recipe->bg_images[UI_STATE_HOVER]);
	if (recipe->bg_images_set[2])
		ft_printf("bg_image_click : %s\n", recipe->bg_images[UI_STATE_CLICK]);
	ft_printf("\n");
}

/*
 * These are done from the first to the last,
 * which means if you have a child recipe
 * it has to be made before the final recipe.
 * Show error to user if they have prefab that cant
 * be found.
*/
void	layout_make_recipes(t_ui_layout *layout)
{
	int				i;
	char			**arr;
	t_ui_recipe	*recipe;

	arr = layout->style_recipe_strings;
	if (!arr)
		return ;
	i = -1;
	while (arr[++i])
	{
		recipe = make_recipe_from_string(layout->windows, layout->elements, layout->recipes, arr[i]);
		if (recipe)
			add_to_list(&layout->recipes, recipe, sizeof(t_ui_recipe));
	}
	// printing to make sure that the recipe is setup correctly;
	/*
	ft_printf("[%s] Print Recipes\n", __FUNCTION__);
	t_list *curr = layout->recipes;
	while (curr)
	{
		ui_recipe_print(curr->content);
		curr = curr->next;
	}
	*/
}

int	make_usable_win_flags(char	**flags)
{
	int		result;
	int		i;

	if (!flags)
		return (0);
	result = 0;
	i = -1;
	while (flags[++i])
	{
		ft_printf("Flag #%d\n", i);
		if (ft_strequ(flags[i], "hide"))
			result |= UI_WINDOW_HIDE;
		else if (ft_strequ(flags[i], "show"))
			result |= UI_WINDOW_SHOW;
		else if (ft_strequ(flags[i], "maximize"))
			result |= UI_WINDOW_MAXIMIZE;
		else if (ft_strequ(flags[i], "minimize"))
			result |= UI_WINDOW_MINIMIZE;
		else if (ft_strequ(flags[i], "fullscreen"))
			result |= UI_WINDOW_FULLSCREEN;
		else if (ft_strequ(flags[i], "fullscreen_windowed"))
			result |= UI_WINDOW_FULLSCREEN_WINDOWED;
		else if (ft_strequ(flags[i], "grab"))
			result |= UI_WINDOW_GRAB;
		else if (ft_strequ(flags[i], "resizeable"))
			result |= UI_WINDOW_RESIZEABLE;
	}
	ft_printf("result : %d\n", result);
	return (result);
}

void	ui_window_edit(t_ui_window *win, t_ui_recipe *recipe)
{
	t_vec4	pos;
	int		i;

	ft_printf("Entering [%s]\n", __FUNCTION__);
	pos = win->pos;
	i = -1;
	while (++i < VEC4_SIZE)
	{
		if (recipe->pos_set[i])
			pos.v[i] = recipe->pos.v[i];
	}
	ui_window_pos_set(win, pos);
	ui_window_texture_pos_set(win, vec2i(pos.w, pos.h));
	if (recipe->title)
		ui_window_title_set(win, recipe->title);
	if (recipe->bg_colors_set[UI_STATE_DEFAULT])
		win->bg_color = recipe->bg_colors[UI_STATE_DEFAULT];

	// TODO Put those xhides into the make_usable...;
	if (ft_strinarr("xhides", recipe->flags))
		win->hide_on_x = 1;
	if (ft_strinarr("xdoesnthides", recipe->flags))
		win->hide_on_x = 0;
	ui_window_flag_set(win, make_usable_win_flags(recipe->flags));

	ui_recipe_print(recipe);
	ui_window_print(win);
	ft_printf("Leaving [%s]\n", __FUNCTION__);
}

/*
 * This function will be called recursively for elements that are contained inside
 * other elements.
*/
void	ui_element_edit(t_ui_element *elem, t_ui_recipe *recipe)
{
	t_vec4	pos;
	int		i;

	pos = elem->pos;
	i = -1;
	while (++i < VEC4_SIZE)
	{
		if (recipe->pos_set[i])
			pos.v[i] = recipe->pos.v[i];
	}
	ui_element_pos_set(elem, pos);
	i = -1;
	while (++i < UI_STATE_AMOUNT)
	{
		if (recipe->bg_colors_set[i])
			ui_element_color_set(elem, i, recipe->bg_colors[i]);
	}
	i = -1;
	while (++i < UI_STATE_AMOUNT)
	{
		if (recipe->bg_images_set[i] && recipe->bg_images[i])
		{
			if (!access(recipe->bg_images[i], F_OK))
				ui_element_image_set_from_path(elem, i, recipe->bg_images[i]);
			else
			{
				int jjj = -1;
				while (elem->win->layout->resource_dirs[++jjj])
				{
					char *temp = ft_sprintf("%s%s", elem->win->layout->resource_dirs[jjj], recipe->bg_images[i]);
					if (!access(temp, F_OK))
					{
						free(recipe->bg_images[i]);
						recipe->bg_images[i] = temp; // dont free temp, since we have just put it in here;!
						break ;
					}
					ft_strdel(&temp);
				}
				ui_element_image_set_from_path(elem, i, recipe->bg_images[i]);
			}
		}
	}
	if (recipe->flags)
	{
		if (ft_strinarr("render_on_parent", recipe->flags))
			elem->render_me_on_parent = 1;
	}
	if (recipe->z_set)
	{
		elem->z = recipe->z;
		elem->figure_out_z = 0;
	}
	if (recipe->show_set)
		elem->show = recipe->show;
	if (g_acceptable[elem->element_type].edit)
		g_acceptable[elem->element_type].edit(elem, recipe);
	else
		ft_printf("[%s] Element of type : %d : %s doenst have a edit function.\n", __FUNCTION__, elem->element_type, ui_element_type_to_string(elem->element_type));
}

/*
 * Applies all the recipes to the corresponding elements.
*/
void	layout_apply_style(t_ui_layout *layout)
{
	t_list			*curr;
	t_ui_window		*win;
	t_ui_element	*elem;
	t_ui_recipe		*recipe;

	curr = layout->windows;
	while (curr)
	{
		win = curr->content;
		recipe = ui_list_get_recipe_by_id(layout->recipes, win->id);
		if (win && recipe)
			ui_window_edit(win, recipe);
		curr = curr->next;
	}
	ft_printf("[%s] Done editing windows.\n", __FUNCTION__);
	curr = layout->elements;
	while (curr)
	{
		elem = curr->content;
		if (!elem)
			ft_printf("[%s] no elem...\n", __FUNCTION__);
		recipe = ui_list_get_recipe_by_id(layout->recipes, elem->id);
		if (recipe)
			ft_printf("[%s] we want to edit with recipe : %s ", __FUNCTION__, recipe->id);
		if (elem && recipe)
			ui_element_edit(elem, recipe);
		else
			ft_printf("[%s] Couldn\'t find recipe for element %s.\n", __FUNCTION__, elem->id);
		ft_printf("success.\n");
		curr = curr->next;
	}
	ft_printf("[%s] Done\n", __FUNCTION__);
}

/*
 * Have this function to make it easier for the user getting a window;
 * (the user doesnt have to know the in the layout the window is...);
*/
t_ui_window	*ui_layout_get_window(t_ui_layout *layout, char *id)
{
	if (!layout)
		return (NULL);
	return (ui_list_get_window_by_id(layout->windows, id));
}

t_ui_element	*ui_layout_get_element(t_ui_layout *layout, char *id)
{
	if (!layout)
		return (NULL);
	return (ui_list_get_element_by_id(layout->elements, id));
}

t_ui_recipe	*ui_layout_get_recipe(t_ui_layout *layout, char *id)
{
	if (!layout)
		return (NULL);
	return (ui_list_get_recipe_by_id(layout->recipes, id));
}

void	ui_family_free(void *family, size_t size)
{
	t_ui_family	*fam;

	fam = family;
	if (!fam)
		return ;
	free(fam->parent_id);
	ft_arraydel(fam->children_strings);
	ft_lstdel(&fam->children, &ui_family_free);
	free(fam);
}

/*
 * Layout shouldnt be malloced, so dont free it;
 * 	(if its malloced, the user has done it, so he has to do it;
*/
void	ui_layout_free(t_ui_layout *layout)
{
	if (!layout)
		return ;
	free(layout->layout_file);
	free(layout->layout_file_content);
	ft_arraydel(layout->layout_element_strings);
	free(layout->style_file);
	free(layout->style_file_content);
	ft_arraydel(layout->style_recipe_strings);
	ft_lstdel(&layout->windows, &ui_window_free);
	ft_lstdel(&layout->elements, &dummy_free_er);
	ft_lstdel(&layout->families, &ui_family_free);
	ft_lstdel(&layout->recipes, &ui_recipe_free);
	ft_arraydel(layout->resource_dirs);
}
