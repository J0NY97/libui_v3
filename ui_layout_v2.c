#include "libui.h"

void	ui_layout_event_v2(t_ui_layout_v2 *layout, SDL_Event e)
{
	t_list			*curr;
	t_ui_window		*win;
	t_ui_element	*elem;

	// Event windows
	curr = layout->windows;
	while (curr)
	{
		win = curr->content;
		if (!win->user_handled_event)
			ui_window_event(win, e);
		curr = curr->next;
	}
	// Event elements
	curr = layout->elements;
	while (curr)
	{
		elem = curr->content;
		if (elem->element_type > 0 && elem->element_type < UI_TYPE_AMOUNT)
			g_acceptable[elem->element_type].eventer(elem, e);
		else
			ft_printf("[%s] Element of type %d (%s) is not supported.\n", __FUNCTION__, elem->element_type, ui_element_type_to_string(elem->element_type));
		curr = curr->next;
	}
}

void	ui_layout_render_v2(t_ui_layout_v2 *layout)
{
	t_list			*curr;
	t_ui_element	*elem;
	t_ui_window		*win;

	// Render The Elements
	curr = layout->elements;
	while (curr)
	{
		elem = curr->content;
		if (elem->element_type > 0 && elem->element_type < UI_TYPE_AMOUNT
			&& g_acceptable[elem->element_type].renderer)
			g_acceptable[elem->element_type].renderer(elem);
		else
			ft_printf("[%s] Element of type %d (%s) is not supported.\n", __FUNCTION__, elem->element_type, ui_element_type_to_string(elem->element_type));
		curr = curr->next;
	}
	// Render The Windows
	curr = layout->windows;
	while (curr)
	{
		win = curr->content;
		ui_window_render(win);
		SDL_RenderPresent(win->renderer);
		curr = curr->next;
	}
}

void	ui_layout_load_v2(t_ui_layout_v2 *layout, char *file)
{
	memset(layout, 0, sizeof(t_ui_layout_v2));

	layout_read_file(layout, file);
	layout_split_elements(layout);
	layout_make_family_trees(layout);
	layout_compile_elements(layout);

	layout_read_style(layout);
	layout_split_styles(layout);
	layout_make_recipes(layout);
	layout_apply_style(layout);
}

int	get_special(t_ui_layout_v2 *layout, char *str)
{
	char	**arr;
	int		result;

	result = 0;
	arr = ft_strsplit(str, ':');
	if (!arr || !arr[0] || !arr[1])
		return (0);
	ft_strtrimwholearr(arr);
	if (ft_strequ(arr[0], "style"))
	{
		layout->style_file = ft_strndup(arr[1], ft_strlen(arr[1]) - 1); // remove ';' from the end;
		result = 1;
	}
	ft_arraydel(arr);
	return (result);
}

char	*get_file_content(t_ui_layout_v2 *layout, char *file)
{
	char	*content;
	char	*line;
	char	*trim;
	size_t	len;
	ssize_t	nread;
	FILE	*fd;

	fd = fopen(file, "r");
	if (!fd)
		return (NULL);
	line = NULL;
	content = NULL;
	while (1)
	{
		nread = getline(&line, &len, fd);
		if (nread == -1)
			break ;
		trim = ft_supertrim(line);
		if (trim != NULL && !get_special(layout, trim)) // whole line was trimmed if is NULL (could happen if the line was a comment);
		{
			ft_stradd(&content, trim);
			ft_strdel(&trim);
		}
		ft_strdel(&line);
	}
	ft_printf("[%s] Content read.\n", __FUNCTION__);
	return (content);
}

void	layout_read_file(t_ui_layout_v2 *layout, char *file)
{
	layout->layout_file = ft_strdup(file);
	layout->layout_file_content = get_file_content(layout, file);
	ft_printf("%s", layout->layout_file_content);
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
void	layout_split_elements(t_ui_layout_v2 *layout)
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
	char		**type_n_name; // dont free this, you can just set them inside the family things;
	int			len;
	int			rm_amount;
	char		*children;

	temp = ft_strsplitfirstoccurenceor(str, '{', ';');
	len = ft_strlen(temp[1]);
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
	type_n_name = ft_strsplit(temp[0], ' ');
	ft_printf("%s %s\n{\n%s\n}\n", type_n_name[0], type_n_name[1], children);
	family->parent_id = type_n_name[1];
	family->parent_type = ui_element_type_from_string(type_n_name[0]);
	family->children_strings = split_string_into_array(children);
	int k = -1;
	while (family->children_strings[++k])
	{
		child = make_family_from_string(family->children_strings[k]);
		if (!child)
			break ;
		add_to_list(&family->children, child, sizeof(t_ui_family));
	}
	ft_arraydel(temp);
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
void	layout_make_family_trees(t_ui_layout_v2 *layout)
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

	elem = ft_memalloc(sizeof(t_ui_element));
	g_acceptable[family->parent_type].maker(win, elem);
	ui_element_parent_set(elem, parent, parent_type);
	ui_element_id_set(elem, family->parent_id);
	add_to_list(list, elem, UI_TYPE_ELEMENT);
	rruc = family->children;
	while (rruc)
	{
		make_elements_from_family(list, win, elem, UI_TYPE_ELEMENT, rruc->content);
		rruc = rruc->next;
	}
}

void	print_all_elements_in_list(t_list *list)
{
	while (list)
	{
		ui_element_print(list->content);
		list = list->next;
	}
}

/*
 * This goes through family and searches for window as the head parent,
 * since all elements need a window, to be created.
*/
void	layout_compile_elements(t_ui_layout_v2 *layout)
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
	print_all_elements_in_list(layout->elements);
}

char	*get_style_content(char *file)
{
	char	*content;
	char	*line;
	char	*trim;
	size_t	len;
	ssize_t	nread;
	FILE	*fd;

	fd = fopen(file, "r");
	if (!fd)
		return (NULL);
	line = NULL;
	content = NULL;
	while (1)
	{
		nread = getline(&line, &len, fd);
		if (nread == -1)
			break ;
		trim = ft_supertrim(line);
		if (trim != NULL) // whole line was trimmed if is NULL (could happen if the line was a comment);
		{
			ft_stradd(&content, trim);
			ft_strdel(&trim);
		}
		ft_strdel(&line);
	}
	ft_printf("[%s] Content read.\n", __FUNCTION__);
	return (content);
}

void	layout_read_style(t_ui_layout_v2 *layout)
{
	if (!layout->style_file)	
		return ;
	layout->style_file_content = get_style_content(layout->style_file);
	ft_printf("%s\n", layout->style_file_content);
}

void	layout_split_styles(t_ui_layout_v2 *layout)
{
	int	k;

	layout->style_recipe_strings = split_string_into_array(layout->style_file_content);
	ft_printf("[%s] Done splitting recipes.\n", __FUNCTION__);
	k = -1;
	while (layout->style_recipe_strings[++k])
		ft_printf("#%d : \n%s\n", k, layout->style_recipe_strings[k]);
}

/*
 * both target and child should always be !NULL;
*/
void	fill_recipe_from_recipe(t_ui_recipe_v2 *target, t_ui_recipe_v2 *child)
{
	if (child->x_set)
	{
		target->pos.x = child->pos.x;
		target->x_set = 1;
	}
	if (child->y_set)
	{
		target->pos.y = child->pos.y;
		target->y_set = 1;
	}
	if (child->w_set)
	{
		target->pos.w = child->pos.w;
		target->w_set = 1;
	}
	if (child->h_set)
	{
		target->pos.h = child->pos.h;
		target->h_set = 1;
	}
}

void	fill_recipe_from_args(t_ui_recipe_v2 *recipe, char **args)
{
	int		i;
	char	**key_value;

	if (!args)
		return ;
	i = -1;
	while (args[++i])
	{
		key_value = ft_strsplit(args[i], ':');
		ft_strtrimwholearr(key_value);
		if (ft_strequ(key_value[0], "x"))
		{
			recipe->pos.x = ft_atof(key_value[1]);
			recipe->x_set = 1;
		}
		else if (ft_strequ(key_value[0], "y"))
		{
			recipe->pos.y = ft_atof(key_value[1]);
			recipe->y_set = 1;
		}
		else if (ft_strequ(key_value[0], "w"))
		{
			recipe->pos.w = ft_atof(key_value[1]);
			recipe->w_set = 1;
		}
		else if (ft_strequ(key_value[0], "h"))
		{
			recipe->pos.h = ft_atof(key_value[1]);
			recipe->h_set = 1;
		}
		else if (ft_strequ(key_value[0], "xywh"))
		{
			char	**pos;

			pos = ft_strsplit(key_value[1], ',');
			recipe->pos.x = ft_atof(pos[0]);
			recipe->pos.y = ft_atof(pos[1]);
			recipe->pos.w = ft_atof(pos[2]);
			recipe->pos.h = ft_atof(pos[3]);
			recipe->x_set = 1;
			recipe->y_set = 1;
			recipe->w_set = 1;
			recipe->h_set = 1;
			ft_arraydel(pos);
		}
		ft_arraydel(key_value);
	}
}

t_ui_recipe_v2	*make_recipe_from_string(t_list *elements, t_list *recipes, char *str)
{
	t_ui_element	*elem;
	t_ui_recipe_v2	*recipe;
	t_ui_recipe_v2	*child_recipe;
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
	args = ft_strsplit(temp[1], ';');
	ft_strtrimwholearr(args);
	ft_arraydel(temp);
	ft_putarr(name_and_prefabs);
	ft_putarr(args);
	recipe = ft_memalloc(sizeof(t_ui_recipe_v2));
	recipe->id = ft_strdup(name_and_prefabs[0]);
	if (elem) // if we have found elem with the same id;
	{
		child_recipe = ui_list_get_recipe_by_id_v2(recipes, g_acceptable[elem->element_type].name);// fill the recipe with the type name recipe;
		if (child_recipe)
			fill_recipe_from_recipe(recipe, child_recipe);
	}
	i = 0; // start at 0 because id is at 0, so we dont want to check that;
	while (name_and_prefabs[++i])
	{
		child_recipe = ui_list_get_recipe_by_id_v2(recipes, name_and_prefabs[i]);
		if (!child_recipe)
		{
			ft_printf("[%s] No recipe with id %s found, couldn\'t copy recipe.\n", name_and_prefabs[i]);
			continue ;
		}
		fill_recipe_from_recipe(recipe, child_recipe);
	}
	fill_recipe_from_args(recipe, args);
	ft_arraydel(name_and_prefabs);
	ft_arraydel(args);
	return (recipe);
}

void	print_recipe(t_ui_recipe_v2 *recipe)
{
	ft_printf("%s\n", recipe->id);	
	print_vec(recipe->pos.v, 4);
}

/*
 * These are done from the first to the last,
 * which means if you have a child recipe
 * it has to be made before the final recipe.
 * Show error to user if they have prefab that cant
 * be found.
*/
void	layout_make_recipes(t_ui_layout_v2 *layout)
{
	int				i;
	char			**arr;
	t_ui_recipe_v2	*recipe;

	arr = layout->style_recipe_strings;
	if (!arr)
		return ;
	i = -1;
	while (arr[++i])
	{
		recipe = make_recipe_from_string(layout->elements, layout->recipes, arr[i]);
		if (recipe)
			add_to_list(&layout->recipes, recipe, sizeof(t_ui_recipe_v2));
	}
	// printing to make sure that the recipe is setup correctly;
	ft_printf("[%s] Print Recipes\n", __FUNCTION__);
	t_list *curr = layout->recipes;
	while (curr)
	{
		print_recipe(curr->content);
		curr = curr->next;
	}
}

void	layout_apply_style(t_ui_layout_v2 *layout)
{
	
}
