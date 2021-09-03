#include "libui.h"
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

t_ui_key_value	*make_key_value(char **orig_arr, char c, int *len)
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
		arr = ft_strsplitfirstoccurence(orig_arr[i], c);
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

void	float_arr_arg_to_float_arr(char *str, float *result_arr, int result_arr_len)
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
		result_arr[i] = atof(arr[i]);
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

int	flag_getter(char *str)
{
	char	**temp;
	int		result;
	int		i;

	temp = ft_strsplit(str, ' ');
	result = 0;
	i = -1;
	while (temp[++i])
	{
		if (ft_strequ(temp[i], "hide"))
			result |= UI_WINDOW_HIDE;
		else if (ft_strequ(temp[i], "show"))
			result |= UI_WINDOW_SHOW;
		else if (ft_strequ(temp[i], "maximize"))
			result |= UI_WINDOW_MAXIMIZE;
		else if (ft_strequ(temp[i], "minimize"))
			result |= UI_WINDOW_MINIMIZE;
		else if (ft_strequ(temp[i], "fullscreen"))
			result |= UI_WINDOW_FULLSCREEN;
		else if (ft_strequ(temp[i], "fullscreen_windowed"))
			result |= UI_WINDOW_FULLSCREEN_WINDOWED;
		else if (ft_strequ(temp[i], "grab"))
			result |= UI_WINDOW_GRAB;
		else if (ft_strequ(temp[i], "resizeable"))
			result |= UI_WINDOW_RESIZEABLE;
	}
	return (result);
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
		else if (ft_strequ(temp[i], "none"))
			result = UI_TEXT_ALIGN_NONE;
	}
	return (result);
}

void	hex_arr_arg_to_uint_arr(char *str, unsigned int *result_arr, bool *result_set, int result_arr_len)
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
		result_set[i] = 1;
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

void	ui_global_get(t_ui_get *get)
{
	int		i;

	get->recipe->type = get->type;
	i = -1;
	while (++i < *get->len)
	{
		if (ft_strstr(get->kv[i].key, "pos"))
		{
			float_arr_arg_to_float_arr(get->kv[i].value, get->recipe->pos.v, 4);
			get->recipe->pos_set = 1;
			get->recipe->pos_info = pos_info_getter(get->kv[i].key);
		}
		else if (ft_strequ(get->kv[i].key, "bg_color"))
		{
			hex_arr_arg_to_uint_arr(get->kv[i].value, get->recipe->bg_color, get->recipe->bg_color_set, 3);
		}
		else if (ft_strequ(get->kv[i].key, "bg_image"))
		{
			str_arr_arg_to_str_arr(get->kv[i].value, get->recipe->bg_image, 3);
			get->recipe->bg_image_set = 1;
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
		else if (ft_strequ(get->kv[i].key, "value"))
		{
			int_arr_arg_to_int_arr(get->kv[i].value, get->recipe->values, 3);
			get->recipe->value_set = 1;
		}
		else if (ft_strequ(get->kv[i].key, "flag"))
		{
			get->recipe->flag = flag_getter(get->kv[i].value);
			get->recipe->flag_set = 1;
		}
		else if (ft_strequ(get->kv[i].key, "tab"))
		{
			char	**temp;
			temp = ft_strsplitfirstoccurence(get->kv[i].value, ',');
			get->recipe->tabs[get->recipe->tab_amount].key = ft_strtrim(temp[0]);
			get->recipe->tabs[get->recipe->tab_amount].value = ft_strtrim(temp[1]);
			get->recipe->tab_amount++;
			ft_arraydel(temp);
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
		while (nread != -1)
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

		t_ui_key_value	*kv = make_key_value(values, ':', &len);
		t_ui_recipe		*recipe = ft_memalloc(sizeof(t_ui_recipe));
		recipe->id = ft_strdup(var_name);
		ft_printf("Getting ID : %s\n", recipe->id);
		ui_global_get(&(t_ui_get){g_acceptable[i].type, &len, kv, recipe});
		add_to_list(&layout->recipes, recipe, sizeof(t_ui_recipe));
		free_key_value(kv, len);
		ft_strdel(&inside);
		ft_arraydel(values);
	}
}

t_ui_recipe	*ui_layout_get_recipe_by_id(t_ui_layout *layout, char *id)
{
	t_list	*curr;

	curr = layout->recipes;
	while (curr)
	{
		if (ft_strequ(((t_ui_recipe *)curr->content)->id, id))
			return (curr->content);
		curr = curr->next;
	}
	ft_printf("[%s] Couldn\'t find recipe with id : %s\n", __FUNCTION__, id);
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
	if (recipe->type == UI_TYPE_LABEL)
		ui_layout_element_edit(&button->label, recipe, layout);
}

void	ui_menu_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *layout)
{
	t_ui_menu		*menu;
	t_ui_element	*temp;
	t_list			*recipes;

	menu = elem->element;
	recipes = layout->recipes;
	ui_layout_element_new(layout, elem->win, recipe);
	temp = ui_layout_get_element_by_id(layout, recipe->id);
	ui_element_parent_set(temp, elem, UI_TYPE_ELEMENT);
	/*
	ui_element_pos_set(temp, recipe->pos);
	print_vec(recipe->pos.v, 4);
	if (ft_strequ(temp->id, "draw_button"))
	{
		ft_printf("recipe->id : %s\n", recipe->id);
		ui_element_print(temp);
	}

	*/
/*
	temp = ui_element_create_from_recipe(elem->win, recipe, layout);
	ui_menu_add(elem, temp, UI_TYPE_ELEMENT);
	ui_element_pos_set(temp, recipe->pos);
	add_to_list(&layout->elements, temp, UI_TYPE_ELEMENT);
	*/
}

void	ui_dropdown_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *layout)
{
	t_ui_dropdown	*drop;

	drop = elem->element;
	if (recipe->type == UI_TYPE_LABEL)
		ui_layout_element_edit(&drop->label, recipe, layout);
	else if (recipe->type == UI_TYPE_MENU)
		ui_layout_element_edit(&drop->menu, recipe, layout);
}

void	ui_input_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *layout)
{
	t_ui_input	*input;

	input = elem->element;
	if (recipe->type == UI_TYPE_LABEL)
		ui_layout_element_edit(&input->label, recipe, layout);
}

void	ui_slider_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *layout)
{
	t_ui_slider	*slider;

	slider = elem->element;
	if (recipe->type == UI_TYPE_BUTTON)
		ui_layout_element_edit(&slider->button, recipe, layout);
	else if (recipe->type == UI_TYPE_LABEL)
	{
		ui_layout_element_edit(&((t_ui_button *)slider->button.element)->label, recipe, layout);
		ui_layout_element_edit(&slider->min_label, recipe, layout);
		ui_layout_element_edit(&slider->max_label, recipe, layout);
	}
}

void	ui_checkbox_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *layout)
{
	(void)elem;
	(void)recipe;
	(void)layout;
}

void	ui_radio_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *layout)
{
	if (recipe->type == UI_TYPE_BUTTON)
		ui_radio_add(elem, ui_element_create_from_recipe(elem->win, recipe, layout));
}

void	ui_tab_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *layout)
{
	(void)elem;
	(void)recipe;
	(void)layout;
}

void	ui_layout_element_edit(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *layout)
{
	int	i;

	// All stuff
	if (recipe->pos_set)
	{
		t_vec4 new_pos;
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
	i = -1;
	while (++i < UI_STATE_AMOUNT)
		if (recipe->bg_color_set[i])
			ui_element_color_set(elem, i, recipe->bg_color[i]);
	if (recipe->bg_image_set)
	{
		ui_element_image_set_from_path(elem, UI_STATE_DEFAULT, recipe->bg_image[UI_STATE_DEFAULT]);
		ui_element_image_set_from_path(elem, UI_STATE_HOVER, recipe->bg_image[UI_STATE_HOVER]);
		ui_element_image_set_from_path(elem, UI_STATE_CLICK, recipe->bg_image[UI_STATE_CLICK]);
	}

	// Slider stuff
	if (recipe->type == UI_TYPE_SLIDER)
	{
		if (recipe->value_set)
		{
			((t_ui_slider *)elem->element)->value = recipe->values[0];
			((t_ui_slider *)elem->element)->min_value = recipe->values[1];
			((t_ui_slider *)elem->element)->max_value = recipe->values[2];
		}
	}

	// Label Stuff
	t_ui_element	*label = NULL;

	if (recipe->type == UI_TYPE_LABEL)
		label = elem;
	else if (recipe->type == UI_TYPE_BUTTON)
		label = &((t_ui_button *)elem->element)->label;
	else if (recipe->type == UI_TYPE_DROPDOWN)
		label = &((t_ui_dropdown *)elem->element)->label;
	else if (recipe->type == UI_TYPE_INPUT)
		label = &((t_ui_input *)elem->element)->label;
	if (label)
	{
		if (recipe->title)
			ui_label_text_set(label, recipe->title);
		if (recipe->font_size_set)
			ui_label_size_set(label, recipe->font_size);
		if (recipe->font_color_set)
			ui_label_color_set(label, recipe->font_color);
		if (recipe->font_path_set)
			ui_label_font_set(label, recipe->font_path);
		if (recipe->text_align_set)
			ui_label_text_align(label, recipe->text_align);
	}

	// Tab stuff
	if (recipe->type == UI_TYPE_TAB)
	{
		t_ui_element	*button;
		t_ui_element	*menu;
		t_ui_tab		*tab;

		i = -1;
		while (++i < recipe->tab_amount)
		{
			ft_printf("[%s] We are trying to make tab from button : <%s> and menu : <%s>.\n", __FUNCTION__, recipe->tabs[i].key, recipe->tabs[i].value);
			button = ui_element_create_from_recipe(elem->win, ui_layout_get_recipe_by_id(layout, recipe->tabs[i].key), layout);
			menu = ui_element_create_from_recipe(elem->win, ui_layout_get_recipe_by_id(layout, recipe->tabs[i].value), layout);
			ui_tab_add(elem, button, menu);
			ft_printf("[%s] Tab created with button id : %s, and menu id : %s\n", __FUNCTION__, button->id, menu->id);
		}
	}

	// Radio stuff
	/*
	if (recipe->type == UI_TYPE_RADIO)
	{
		t_ui_element	*rad_butt;

		while (++i < recipe->radio_amount)
		{
			rad_butt = ui_element_create_from_recipe(elem->win, get_recipe_by_id(layout->recipes, recipe->radios[i]), layout);
		}
	}
	*/
}

void	ui_populate_recipe(t_ui_recipe *result, t_ui_recipe from)
{
	int	i;

	if (result->id && from.id)
	{
		ft_strdel(&result->id);
		result->id = ft_strdup(from.id);
	}
	result->type = from.type;
	if	(from.pos_set)
	{
		t_vec4 new_pos;
	
		i = -1;
		new_pos = from.pos;
		if (from.pos_info & UI_POS_X)
		{
			result->pos_info |= UI_POS_X;
			new_pos.x = from.pos.v[++i];
		}
		if (from.pos_info & UI_POS_Y)
		{
			result->pos_info |= UI_POS_Y;
			new_pos.y = from.pos.v[++i];
		}
		if (from.pos_info & UI_POS_W)
		{
			result->pos_info |= UI_POS_W;
			new_pos.w = from.pos.v[++i];
		}
		if (from.pos_info & UI_POS_H)
		{	
			result->pos_info |= UI_POS_H;
			new_pos.h = from.pos.v[++i];
		}
		result->pos = new_pos;
		result->pos_set = 1;
	}
	i = -1;
	while (++i < UI_STATE_AMOUNT)
	{
		if (from.bg_color_set[i])
		{
			result->bg_color_set[i] = from.bg_color_set[i];
			result->bg_color[i] = from.bg_color[i];
		}
	}
	if (from.bg_image_set)
	{
		i = -1;
		result->bg_image_set = from.bg_image_set;
		while (++i < UI_STATE_AMOUNT)
			result->bg_image[i] = from.bg_image[i];
	}
	if (from.title)
	{
		if (result->title)
			ft_strdel(&result->title);
		result->title = ft_strdup(from.title);
	}
	if (from.font_path_set && from.font_path)
	{
		if (result->font_path)
			ft_strdel(&result->font_path);
		result->font_path = ft_strdup(from.font_path);
		result->font_path_set = from.font_path_set;
	}
	if (from.font_size_set)
	{
		result->font_size = from.font_size;
		result->font_size_set = from.font_size_set;
	}
	if (from.text_align_set)
	{
		result->text_align = from.text_align;
		result->text_align_set = from.text_align_set;
	}
	if (from.value_set)
	{
		result->value_set = from.value_set;
		i = -1;
		while (++i < 3)
			result->values[i] = from.values[i];
	}
}

t_ui_element	*ui_element_create_from_recipe(t_ui_window *win, t_ui_recipe *recipe, t_ui_layout *layout)
{
	t_ui_element	*elem;
	t_ui_recipe		*child_recipe;
	int				j;
	t_ui_recipe		final_recipe;

	if (!recipe)
		ft_printf("[%s] Recipe is NULL... error btw\n", __FUNCTION__);
	if (recipe->type >= UI_TYPE_AMOUNT || recipe->type < 0)
		ft_printf("[%s] Recipe type <%d> is not supported.\n", __FUNCTION__, recipe->type);
	memset(&final_recipe, 0, sizeof(t_ui_recipe));
	elem = ft_memalloc(sizeof(t_ui_element));
	g_acceptable[recipe->type].maker(win, elem);
	elem->id = ft_strdup(recipe->id);
	j = -1;
	while (++j < recipe->child_amount)
	{
		child_recipe = ui_layout_get_recipe_by_id(layout, recipe->children_ids[j]);
		if (child_recipe)
		{
			ft_printf("[%s] We have found child recipe : %s\n", __FUNCTION__, child_recipe->id);
			if (child_recipe->type == UI_TYPE_ELEMENT)
			{
//				ui_populate_recipe(&final_recipe, *child_recipe);
				continue ;
				ui_layout_element_edit(elem, child_recipe, layout);
			}
			else if (g_acceptable[recipe->type].editor)
				g_acceptable[recipe->type].editor(elem, child_recipe, layout);
			else
				ft_printf("[%s] No editor made for element type %d.\n", __FUNCTION__, recipe->type);
		}
		else
			ft_printf("[%s] When searching for child, Couldnt find recipe with id : %s\n", __FUNCTION__, recipe->children_ids[j]);
	}
	/*
	ui_populate_recipe(&final_recipe, *recipe);
	ui_populate_recipe(recipe, final_recipe);
	if (ft_strequ(elem->id, "draw_button"))
	{
		ft_printf("draw_button recipe final pos : ");
		print_vec(final_recipe.pos.v, 4);
		print_vec(recipe->pos.v, 4);
	}
	*/
	j = -1;
	while (++j < recipe->child_amount)
	{
		child_recipe = ui_layout_get_recipe_by_id(layout, recipe->children_ids[j]);
		if (child_recipe->type == UI_TYPE_ELEMENT)
			ui_layout_element_edit(elem, child_recipe, layout);
	}

	ui_layout_element_edit(elem, recipe, layout);
	return (elem);
}

void	ui_layout_element_new(t_ui_layout *layout, t_ui_window *win, t_ui_recipe *recipe)
{
	t_ui_element	*elem;

	elem = ui_element_create_from_recipe(win, recipe, layout);
	if (!elem)
	{
		ft_printf("[%s] Element couldnt be created from....\n", __FUNCTION__);
		return ;
	}
	ft_printf("[%s] Successful make of %s.\n", __FUNCTION__, recipe->id);
	add_to_list(&layout->elements, elem, UI_TYPE_ELEMENT);
	ft_printf("[%s] Successful adding elem to list.\n", __FUNCTION__);
}

void	ui_layout_window_new(t_ui_layout *layout, t_ui_recipe *recipe)
{
	t_ui_window	*window;
	t_ui_recipe	*child_recipe;
	int			i;

	// Edit
	window = malloc(sizeof(t_ui_window));
	ui_window_new(window, recipe->title, recipe->pos);
	ui_window_flag_set(window, recipe->flag);
	window->id = ft_strdup(recipe->id);
	ui_texture_fill(window->renderer, window->texture, recipe->bg_color[UI_STATE_DEFAULT]);
	add_to_list(&layout->windows, window, UI_TYPE_WINDOW);
	// END Edit

	i = -1;
	ft_printf("Child Amount : %d\n", recipe->child_amount);
	while (++i < recipe->child_amount)
	{
		ft_printf("#%d %s\n", i, recipe->children_ids[i]);
		child_recipe = ui_layout_get_recipe_by_id(layout, recipe->children_ids[i]);
		if (child_recipe)
		{
			ft_printf("[ui_layout_window_new] Trying to make: %s\n", child_recipe->id);
			ui_layout_element_new(layout, window, child_recipe);
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
	int			win_found;
	t_list		*curr;
	t_ui_recipe	*recipe;

	win_found = 0;
	curr = layout->recipes;
	recipe = NULL;
	while (curr)
	{
		recipe = curr->content;
		if (recipe->type == UI_TYPE_WINDOW)
		{
			ft_printf("Window Found : %s\n", recipe->id);
			ui_layout_window_new(layout, recipe);
			win_found = 1;
		}
		curr = curr->next;
	}
	if (!win_found)
		ft_printf("[%s] Window Not Found!\n", __FUNCTION__);
}

int	ui_layout_load(t_ui_layout *layout, char *ui_file_path)
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
		return (0);
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
	return (1);
}
