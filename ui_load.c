#include "libui_v3.h"
#include "ui_load.h"

/*
 * Everything in Menu/Window that are not in the accepted,
 * are considered as already made elements in the ui file,
 * and will be tried to ui_element_get_by_id from a list of elements.
 *
 * If Button has something that is not in the accepted,
 * it will assume its a label, and try to find that label id
 * from the list of elements.
*/

void	ui_print_accepted(void)
{
	int	i;
	int	j;

	i = -1;
	while (++i < UI_ACCEPT_AMOUNT)
	{
		j = -1;
		ft_printf("\n[%s]\n{\n", g_acceptable[i].name);
		while (g_acceptable[i].values && g_acceptable[i].values[++j])
		{
			ft_printf(" %s\n", g_acceptable[i].values[j]);
		}
		ft_printf("}\n");
	}
}

char	**ft_strsplitfirstoccurence(char *str, char c)
{
	char	**arr;
	int		i;

	if (!str)
		return (NULL);
	arr = ft_memalloc(sizeof(char *) * 3);
	i = -1;
	while (str[++i] && str[i] != c);
	arr[0] = ft_strsub(str, 0, i);
	if (str[i] == c)
		arr[1] = ft_strsub(str, i + 1, ft_strlen(str) - 1);
	return (arr);
}

/*
 * Description in function name.
*/
char	**ft_strsplitthatchecksifcharinsidestring(char *str, char c)
{
	char	**arr;
	int		count;
	int		i;
	int		last;
	int		has_been_hash;

	if (!str)
		return (NULL);
	count = 0;
	i = -1;
	last = 0;
	has_been_hash = 0;
	arr = malloc(sizeof(char *) * 1);
	while (str[++i])
	{
		if (str[i] == '"')
			has_been_hash++;
		if (str[i] == c && has_been_hash % 2 == 0)
		{
			arr = realloc(arr, sizeof(char *) * ++count);
			arr[count - 1] = ft_strsub(str, last, i - last);
			last = i + 1;
		}
	}
	if (i != last) // because if the c == last char in str (not variable last, the actual last character)
	{
		arr = realloc(arr, sizeof(char *) * ++count);
		arr[count - 1] = ft_strsub(str, last, i - last);
	}
	arr = realloc(arr, sizeof(char *) * ++count);
	arr[count - 1] = NULL;
	return (arr);
}

t_ui_key_value	*make_key_value(char **orig_arr, int *len)
{
	t_ui_key_value	*kv;
	char			**arr;
	int				i;
	char			*temp;

	i = -1;
	while (orig_arr[++i]);
	kv = ft_memalloc(sizeof(t_ui_key_value) * (i + 1));
	i = -1;
	while (orig_arr[++i])
	{
		arr = ft_strsplitfirstoccurence(orig_arr[i], ':');
		temp = ft_strtrim(arr[0]);
		kv[i].key = ft_strdup(temp);
		ft_strdel(&temp);
		if (arr[1])
		{
			temp = ft_strtrim(arr[1]);
			kv[i].value = ft_strdup(temp);
			ft_strdel(&temp);
		}
		else
			kv[i].value = NULL;
		ft_arraydel(arr);
	}
	*len = i;
	return (kv);
}

void	free_key_value(t_ui_key_value *kv, int len)
{
	int	i;	

	i = -1;
	while (++i < len)
	{
		ft_strdel(&kv[i].key);
		ft_strdel(&kv[i].value);
	}
	free(kv);
}

void	int_arr_arg_to_int_arr(char *str, int *result_arr, int result_arr_len)
{
	char	*trimmed;
	char	**arr;
	int		i;

	trimmed = ft_strtrim(str);
	arr = ft_strsplit(trimmed, ',');
	i = -1;
	while (arr[++i] && i < result_arr_len)
	{
		if (ft_strequ(arr[i], "NULL"))
			continue ;
		result_arr[i] = ft_atoi(arr[i]);
	}
	ft_arraydel(arr);
}

int	pos_info_getter(char *str)
{
	char	**temp;
	int		pos_info;

	temp = ft_strsplitfirstoccurence(str, '.');	
	pos_info = 0;
	if (temp[1])
	{
		if (ft_strchr(temp[1], 'x'))	
			pos_info |= UI_POS_X;
		if (ft_strchr(temp[1], 'y'))	
			pos_info |= UI_POS_Y;
		if (ft_strchr(temp[1], 'w'))	
			pos_info |= UI_POS_W;
		if (ft_strchr(temp[1], 'h'))	
			pos_info |= UI_POS_H;
	}
	else
		pos_info = UI_POS_X | UI_POS_Y | UI_POS_W | UI_POS_H;
	ft_arraydel(temp);
	return (pos_info);
}

void	pos_arr_getter(char *str, int *result_arr, int pos_info)
{
	char	*trimmed;
	char	**arr;
	int		i;

	trimmed = ft_strtrim(str);
	arr = ft_strsplit(trimmed, ',');
	i = -1;
	while (arr[++i])
	{
		if (ft_strequ(arr[i], "NULL"))
			continue ;
		if (pos_info & UI_POS_X)
		{
			result_arr[0] = ft_atoi(arr[i]);
			pos_info ^= UI_POS_X;
		}
		else if (pos_info & UI_POS_Y)
		{
			result_arr[1] = ft_atoi(arr[i]);
			pos_info ^= UI_POS_Y;
		}
		else if (pos_info & UI_POS_W)
		{
			result_arr[2] = ft_atoi(arr[i]);
			pos_info ^= UI_POS_W;
		}
		else if (pos_info & UI_POS_H)
		{
			result_arr[3] = ft_atoi(arr[i]);
			pos_info ^= UI_POS_H;
		}
	}
	ft_arraydel(arr);
}

int	text_align_getter(char *str)
{
	char	**temp;
	int		result;
	int		i;

	temp = ft_strsplit(str, ' ');
	result = 0;
	i = -1;
	while (temp[++i])
	{
		if (ft_strequ(temp[i], "center"))
			result |= UI_TEXT_ALIGN_CENTER;
		else if (ft_strequ(temp[i], "top"))
			result |= UI_TEXT_ALIGN_TOP;
		else if (ft_strequ(temp[i], "bot"))
			result |= UI_TEXT_ALIGN_BOT;
		else if (ft_strequ(temp[i], "left"))
			result |= UI_TEXT_ALIGN_LEFT;
		else if (ft_strequ(temp[i], "right"))
			result |= UI_TEXT_ALIGN_RIGHT;
	}
	return (result);
}

void	hex_arr_arg_to_uint_arr(char *str, unsigned int *result_arr, int result_arr_len)
{
	char	*trimmed;
	char	**arr;
	int		i;

	trimmed = ft_strtrim(str);
	arr = ft_strsplit(trimmed, ',');
	i = -1;
	while (arr[++i] && i < result_arr_len)
	{
		if (ft_strequ(arr[i], "NULL"))
			continue ;
		result_arr[i] = (unsigned int)strtoul(arr[i], NULL, 16);
	}
	ft_arraydel(arr);
}

/*
 * Not meaning normal C string.
 * but from the ui file '"string"'
*/
char	*trim_string(char *str)
{
	char	*result;
	int		first;
	int		second;

	result = NULL;
	if (!str)
		return (NULL);
	first = ft_strchrlen(str, '"') + 1;
	second = ft_strchrlen(str + first, '"');
	result = ft_strsub(str, first, second);
	return (result);
}

void	str_arr_arg_to_str_arr(char *str, char **result_arr, int result_arr_len)
{
	char	*trimmed;
	char	*trimtrim;
	char	**arr;
	int		i;

	trimmed = ft_strtrim(str);
	arr = ft_strsplit(trimmed, ',');
	i = -1;
	while (arr[++i] && i < result_arr_len)
	{
		if (ft_strequ(arr[i], "NULL"))
			continue ;
		trimtrim = trim_string(arr[i]);
		result_arr[i] = ft_strdup(trimtrim);
		ft_strdel(&trimtrim);
	}
	ft_arraydel(arr);
}

void	ui_element_get(t_ui_get *get)
{
	int	i;

	get->recipe->type = UI_TYPE_ELEMENT;
	i = -1;
	while (++i < *get->len)
	{
		if (ft_strequ(get->kv[i].key, "pos"))
		{
			int_arr_arg_to_int_arr(get->kv[i].value, get->recipe->pos.v, 4);
			get->recipe->pos_set = 1;
			get->recipe->pos_info = pos_info_getter(get->kv[i].key);
		}
		else if (ft_strequ(get->kv[i].key, "bg_color"))
		{
			hex_arr_arg_to_uint_arr(get->kv[i].value, get->recipe->bg_color, 3);
			get->recipe->bg_color_set = 1;
		}
		else if (ft_strequ(get->kv[i].key, "bg_image"))
		{
			str_arr_arg_to_str_arr(get->kv[i].value, get->recipe->bg_image, 3);
			get->recipe->bg_image_set = 1;
		}
		else
			ft_printf("[ui_element_get] %s arg is not supported.\n", get->kv[i].key);
	}
}

void	ui_dropdown_get(t_ui_get *get)
{
	int	i;

	get->recipe->type = UI_TYPE_DROPDOWN;
	i = -1;
	while (++i < *get->len)
	{
		if (ft_strequ(get->kv[i].key, "pos"))
		{
			int_arr_arg_to_int_arr(get->kv[i].value, get->recipe->pos.v, 4);
			get->recipe->pos_set = 1;
			get->recipe->pos_info = pos_info_getter(get->kv[i].key);
		}
		else // should be variables, button and menu
		{
			get->recipe->children_ids = realloc(get->recipe->children_ids, sizeof(char *) * (++get->recipe->child_amount + 1));
			get->recipe->children_ids[get->recipe->child_amount - 1] = ft_strdup(get->kv[i].key);
		}
	}
}

void	ui_menu_get(t_ui_get *get)
{
	int	i;

	get->recipe->type = UI_TYPE_MENU;
	i = -1;
	while (++i < *get->len)
	{
		ft_printf("%s : %s\n", get->kv[i].key, get->kv[i].value);
		if (ft_strequ(get->kv[i].key, "pos"))
		{
			int_arr_arg_to_int_arr(get->kv[i].value, get->recipe->pos.v, 4);
			get->recipe->pos_set = 1;
			get->recipe->pos_info = pos_info_getter(get->kv[i].key);
		}
		else if (ft_strequ(get->kv[i].key, "bg_color"))
		{
			hex_arr_arg_to_uint_arr(get->kv[i].value, get->recipe->bg_color, 3);
			get->recipe->bg_color_set = 1;
		}
		else // should be variables
		{
			get->recipe->children_ids = realloc(get->recipe->children_ids, sizeof(char *) * (++get->recipe->child_amount + 1));
			get->recipe->children_ids[get->recipe->child_amount - 1] = ft_strdup(get->kv[i].key);
		}
	}
}

void	ui_window_get(t_ui_get *get)
{
	int	i;

	get->recipe->type = UI_TYPE_WINDOW;
	i = -1;
	get->recipe->title = NULL;
	while (++i < *get->len)
	{
		ft_printf("%s : %s\n", get->kv[i].key, get->kv[i].value);
		if (ft_strequ(get->kv[i].key, "pos"))
		{
			int_arr_arg_to_int_arr(get->kv[i].value, get->recipe->pos.v, 4);
			get->recipe->pos_set = 1;
		}
		else if (ft_strequ(get->kv[i].key, "title"))
		{
			get->recipe->title = trim_string(get->kv[i].value);
		}
		else if (ft_strequ(get->kv[i].key, "bg_color"))
		{
			get->recipe->bg_color[UI_STATE_DEFAULT] = strtoul(get->kv[i].value, NULL, 16);
			get->recipe->bg_color_set = 1;
		}
		else // should be variables
		{
			get->recipe->children_ids = realloc(get->recipe->children_ids, sizeof(char *) * (++get->recipe->child_amount + 1));
			get->recipe->children_ids[get->recipe->child_amount - 1] = ft_strdup(get->kv[i].key);
		}
	}
}

void	ui_label_get(t_ui_get *get)
{
	int	i;

	get->recipe->type = UI_TYPE_LABEL;
	i = -1;
	while (++i < *get->len)
	{
		ft_printf("%s : %s\n", get->kv[i].key, get->kv[i].value);
		if (ft_strequ(get->kv[i].key, "pos"))
		{
			int_arr_arg_to_int_arr(get->kv[i].value, get->recipe->pos.v, 4);
			get->recipe->pos_set = 1;
			get->recipe->pos_info = pos_info_getter(get->kv[i].key);
		}
		else if (ft_strequ(get->kv[i].key, "title"))
		{
			get->recipe->title = trim_string(get->kv[i].value);
		}
		else if (ft_strequ(get->kv[i].key, "font_path"))
		{
			get->recipe->font_path = trim_string(get->kv[i].value);
			get->recipe->font_path_set = 1;
		}
		else if (ft_strequ(get->kv[i].key, "font_size"))
		{
			get->recipe->font_size = ft_atoi(get->kv[i].value);
			get->recipe->font_size_set = 1;
		}
		else if (ft_strequ(get->kv[i].key, "font_color"))
		{
			get->recipe->font_color = strtoul(get->kv[i].value, NULL, 16);
			get->recipe->font_color_set = 1;
		}
		else if (ft_strequ(get->kv[i].key, "text_align"))
		{
			get->recipe->text_align = text_align_getter(get->kv[i].value);
			get->recipe->text_align_set = 1;
		}

	}
}

void	ui_button_get(t_ui_get *get)
{
	int		i;
	int		pos_info;
	char	**temp_arr;

	get->recipe->type = UI_TYPE_BUTTON;
	i = -1;
	pos_info = 0;
	while (++i < *get->len)
	{
		if (ft_strstr(get->kv[i].key, "pos"))
		{
			int_arr_arg_to_int_arr(get->kv[i].value, get->recipe->pos.v, 4);
			get->recipe->pos_set = 1;
			get->recipe->pos_info = pos_info_getter(get->kv[i].key);

			ft_printf("pos : %d %d %d %d\n", get->recipe->pos.x, get->recipe->pos.y, get->recipe->pos.w, get->recipe->pos.h);
		}
		else if (ft_strequ(get->kv[i].key, "bg_color"))
		{
			hex_arr_arg_to_uint_arr(get->kv[i].value, get->recipe->bg_color, 3);
			get->recipe->bg_color_set = 1;

			ft_printf("bg_color : %#.8x %#.8x %#.8x\n",
				get->recipe->bg_color[UI_STATE_DEFAULT],
				get->recipe->bg_color[UI_STATE_HOVER],
				get->recipe->bg_color[UI_STATE_CLICK]);
		}
		else if (ft_strequ(get->kv[i].key, "bg_image"))
		{
			str_arr_arg_to_str_arr(get->kv[i].value, get->recipe->bg_image, 3);
			get->recipe->bg_image_set = 1;

			ft_printf("bg_image : %s %s %s\n",
				get->recipe->bg_image[UI_STATE_DEFAULT],
				get->recipe->bg_image[UI_STATE_HOVER],
				get->recipe->bg_image[UI_STATE_CLICK]
				);
		}
		else if (ft_strequ(get->kv[i].key, "title"))
		{
			get->recipe->title = trim_string(get->kv[i].value);
		}
		else // should be variables
		{
			get->recipe->children_ids = realloc(get->recipe->children_ids, sizeof(char *) * (++get->recipe->child_amount + 1));
			get->recipe->children_ids[get->recipe->child_amount - 1] = ft_strdup(get->kv[i].key);
		}
	}
}

void	decide(t_ui_layout *layout, char *str, char *var_name, FILE *fd)
{
	char	*line;
	size_t	len;
	ssize_t	nread;
	int		result;
	char	*trimmed;
	int		open_parentheses;
	int		i;
	char	*inside;
	char	**values;

	result = -1;
	i = -1;
	while (++i < UI_ACCEPT_AMOUNT)
	{
		if (ft_strequ(str, g_acceptable[i].name))
		{
			result = g_acceptable[i].type;
			break ;
		}
	}
	if (result != -1)
	{
		inside = NULL;
		open_parentheses = 0;
		line = NULL;
		nread = getline(&line, &len, fd);
		while (1)
		{
			if (ft_strchr(line, '}'))
				open_parentheses--;
			if (ft_strchr(line, '{'))
				open_parentheses++;
			trimmed = ft_strtrim(line);
			ft_stradd(&inside, trimmed);
			ft_strdel(&trimmed);
			ft_strdel(&line);
			if (open_parentheses == 0)
				break ;
			nread = getline(&line, &len, fd);
		}
		ft_strdel(&line);
		trimmed = ft_strsub(inside, 1, ft_strlen(inside) - 2);
		ft_strdel(&inside);
		inside = ft_strdup(trimmed);
		ft_strdel(&trimmed);

		values = ft_strsplitthatchecksifcharinsidestring(inside, ';');
		int len;

		t_ui_key_value	*kv = make_key_value(values, &len);
		t_ui_recipe		*recipe = ft_memalloc(sizeof(t_ui_recipe));
		recipe->id = ft_strdup(var_name);
		ft_printf("Getting ID : %s\n", recipe->id);
		if (!g_acceptable[i].getter)
			ft_printf("[decide] No getter for %s made.\n", g_acceptable[i].name);
		else
		{
			g_acceptable[i].getter(&(t_ui_get){&len, kv, recipe});
			add_to_list(&layout->recipes, recipe, sizeof(t_ui_recipe));
		}
		free_key_value(kv, len);
		ft_strdel(&inside);
		ft_arraydel(values);
	}
}

t_ui_recipe	*get_recipe_by_id(t_list *list, char *id)
{
	t_list	*curr;

	curr = list;
	while (curr)
	{
		if (ft_strequ(((t_ui_recipe *)curr->content)->id, id))
			return (curr->content);
		curr = curr->next;
	}
	return (NULL);
}

////////////////////
// Editors
////////////////////

void	ui_label_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *layout)
{
	(void)elem;
	(void)recipe;
	(void)layout;
}

void	ui_button_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *layout)
{
	t_ui_button	*button;

	button = elem->element;
	(void)layout;
	if (recipe->type == UI_TYPE_LABEL)
		ui_layout_element_edit(&button->label, recipe);
}

void	ui_menu_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *layout)
{
	t_ui_menu	*menu;
	t_list		*recipes;

	recipes = layout->recipes;
	menu = elem->element;
	ui_layout_element_new(layout, elem->win, recipe, recipes);
	ui_element_parent_set(ui_layout_get_element_by_id(layout, recipe->id), elem, UI_TYPE_ELEMENT, &elem->show);
}

void	ui_dropdown_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *layout)
{
	t_ui_dropdown	*drop;
	t_ui_button		*button;

	drop = elem->element;
	button = drop->button.element;
	if (recipe->type == UI_TYPE_LABEL)
		ui_layout_element_edit(&button->label, recipe);
	else if (recipe->type == UI_TYPE_BUTTON)
		ui_layout_element_edit(&drop->button, recipe);
	else if (recipe->type == UI_TYPE_MENU)
		ui_layout_element_edit(&drop->menu, recipe);
	(void)layout;
}

void	ui_layout_element_edit(t_ui_element *elem, t_ui_recipe *recipe)
{
	// All stuff
	if (recipe->pos_set)
	{
		t_vec4i new_pos;
		int		i;
	
		i = -1;
		new_pos = elem->pos;
		if (recipe->pos_info & UI_POS_X)
			new_pos.x = recipe->pos.v[++i];
		if (recipe->pos_info & UI_POS_Y)
			new_pos.y = recipe->pos.v[++i];
		if (recipe->pos_info & UI_POS_W)
			new_pos.w = recipe->pos.v[++i];
		if (recipe->pos_info & UI_POS_H)
			new_pos.h = recipe->pos.v[++i];
		ui_element_pos_set(elem, new_pos);
	}
	if (recipe->bg_color_set)
	{
		ui_element_color_set(elem, UI_STATE_DEFAULT, recipe->bg_color[UI_STATE_DEFAULT]);
		ui_element_color_set(elem, UI_STATE_HOVER, recipe->bg_color[UI_STATE_HOVER]);
		ui_element_color_set(elem, UI_STATE_CLICK, recipe->bg_color[UI_STATE_CLICK]);
	}
	if (recipe->bg_image_set)
	{
		ui_element_image_set_from_path(elem, UI_STATE_DEFAULT, recipe->bg_image[UI_STATE_DEFAULT]);
		ui_element_image_set_from_path(elem, UI_STATE_HOVER, recipe->bg_image[UI_STATE_HOVER]);
		ui_element_image_set_from_path(elem, UI_STATE_CLICK, recipe->bg_image[UI_STATE_CLICK]);
	}
	// Label Stuff
	if (recipe->title)
	{
		if (recipe->type == UI_TYPE_LABEL)
			ui_label_text_set(elem, recipe->title);
		else if (recipe->type == UI_TYPE_BUTTON)
			ui_label_text_set(&((t_ui_button *)elem->element)->label, recipe->title);
	}
	if (recipe->font_size_set)
		ui_label_size_set(elem, recipe->font_size);
	if (recipe->font_color_set)
		ui_label_color_set(elem, recipe->font_color);
	if (recipe->font_path_set)
		ui_label_font_set(elem, recipe->font_path);
	if (recipe->text_align_set)
		ui_label_text_align(elem, recipe->text_align);
}

void	ui_layout_element_new(t_ui_layout *layout, t_ui_window *win, t_ui_recipe *recipe, t_list *recipes)
{
	t_ui_element	*elem;	
	t_ui_recipe		*child_recipe;
	int				i;
	int				j;

	i = -1;
	while (++i < UI_ACCEPT_AMOUNT)
	{
		if (g_acceptable[i].type == recipe->type)
		{
			elem = ft_memalloc(sizeof(t_ui_element));
			g_acceptable[i].maker(win, elem);
			elem->id = ft_strdup(recipe->id);
			j = -1;
			ft_printf("[ui_layout_element_new] Element has %d children.\n", recipe->child_amount);
			while (++j < recipe->child_amount)
			{
				child_recipe = get_recipe_by_id(recipes, recipe->children_ids[j]);
				if (child_recipe)
				{
					ft_printf("[ui_layout_element_new] We have found child recipe : %s\n", child_recipe->id);
					if (child_recipe->type == UI_TYPE_ELEMENT)
						ui_layout_element_edit(elem, child_recipe);
					else if (g_acceptable[i].editor)
						g_acceptable[i].editor(elem, child_recipe, layout);
					else
						ft_printf("[ui_layout_element_new] No editor made for element type %d.\n", recipe->type);
				}
				else
					ft_printf("[ui_layout_element_new] When searching for child, Couldnt find recipe with id : %s\n", recipe->children_ids[j]);
			}
			ui_layout_element_edit(elem, recipe);
			add_to_list(&layout->elements, elem, UI_TYPE_ELEMENT);
			ft_printf("[ui_layout_element_new] Successful make of %s.\n", recipe->id);
			return ;
		}
	}
	ft_printf("Failed to make new layout element.\n");
}

void	ui_layout_window_new(t_ui_layout *layout, t_ui_recipe *recipe)
{
	t_ui_window	*window;
	t_ui_recipe	*child_recipe;
	int			i;

	window = malloc(sizeof(t_ui_window));
	ui_window_new(window, recipe->title, recipe->pos);
	window->id = ft_strdup(recipe->id);
	ui_texture_fill(window->renderer, window->texture, recipe->bg_color[UI_STATE_DEFAULT]);
	add_to_list(&layout->windows, window, UI_TYPE_WINDOW);

	i = -1;
	ft_printf("Child Amount : %d\n", recipe->child_amount);
	while (++i < recipe->child_amount)
	{
		ft_printf("#%d %s\n", i, recipe->children_ids[i]);
		child_recipe = get_recipe_by_id(layout->recipes, recipe->children_ids[i]);
		if (child_recipe)
		{
			ft_printf("[ui_layout_window_new] Trying to make: %s\n", child_recipe->id);
			ui_layout_element_new(layout, window, child_recipe, layout->recipes);
			ft_printf("[ui_layout_window_new] Was succesful to make: %s\n", child_recipe->id);
		}
		else
			ft_printf("[ui_layout_window_new] Couldnt find: %s\n", recipe->children_ids[i]);
	}
}

/*
 * 1. find window.
 * 2. make it.
 * 3. find elements.
 * 4. make them.
 * 5. goto 1.
*/
void	compile_recipes(t_ui_layout *layout)
{
	t_list	*curr;
	t_ui_recipe	*recipe;

	curr = layout->recipes;
	recipe = NULL;
	while (curr)
	{
		if (((t_ui_recipe *)curr->content)->type == UI_TYPE_WINDOW)
		{
			recipe = curr->content;
			ft_printf("Window Found : %s\n", recipe->id);
			ui_layout_window_new(layout, recipe);
		}
		curr = curr->next;
	}
	if (!recipe)
		ft_printf("[compile_recipes] Window Not Found!\n");
}

void	ui_load(t_ui_layout *layout, char *ui_file_path)
{
	FILE	*fd;
	char	*line;
	char	*trimmed;
	char	**arr;
	size_t	len;
	ssize_t	nread;

	memset(layout, 0, sizeof(t_ui_layout));
	fd = fopen(ui_file_path, "r");
	if (!fd)
	{
		ft_printf("[ui_load] Couldnt open ui file : %s\n", ui_file_path);
		return;
	}
	else
		ft_printf("[ui_load] Could open ui file : %s\n", ui_file_path);
	line = NULL;
	nread = getline(&line, &len, fd);
	while (nread != -1)
	{
		ft_printf("nread : %d\n", nread);
		trimmed = ft_strtrim(line);
		arr = ft_strsplit(trimmed, ' ');
		if (arr[0] && arr[1])
			decide(layout, arr[0], arr[1], fd);
		ft_strdel(&line);
		ft_strdel(&trimmed);
		ft_arraydel(arr);
		nread = getline(&line, &len, fd);
	}
	fclose(fd);
	compile_recipes(layout);
	ft_printf("Done reading : %s\n", ui_file_path);
}
