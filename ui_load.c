#include "libui_v3.h"

/*
 * Fun fact: you can only have flexible array in the end of a struct.
 * 	which actually makes sense if you think about it.
*/
typedef struct s_ui_acceptable
{
	char		*name;
	int			type;
	void		(*freer)(void *args);
	void		(*getter)(t_ui_get *get);
	const char	**values;
}				t_ui_acceptable;

static const char *g_accepted_button[] = {
	"pos",
	"bg_color",
	"bg_image",
	"Label",
	NULL
};

static const char *g_accepted_window[] = {
	"pos",
	"bg_color",
	"title",
	NULL
};

static const char *g_accepted_menu[] = {
	"pos",
	NULL
};

static const char *g_accepted_label[] = {
	"pos",
	"font_path",
	"font_size",
	"font_color",
	NULL
};

static const t_ui_acceptable	g_acceptable_button =
{
	.name = "Button",
	.type = UI_TYPE_BUTTON,
	.freer = &ui_button_free,
	.getter = &ui_button_get,
	.values = g_accepted_button
};

static const t_ui_acceptable	g_acceptable_label =
{
	.name = "Label",
	.type = UI_TYPE_LABEL,
	.freer = &ui_label_free,
	.getter = &ui_label_get,
	.values = g_accepted_label
};

static const t_ui_acceptable	g_acceptable_menu =
{
	.name = "Menu",
	.type = UI_TYPE_MENU,
	.freer = &ui_menu_free,
	.getter = &ui_menu_get,
	.values = g_accepted_menu
};

static const t_ui_acceptable	g_acceptable_window =
{
	.name = "Window",
	.type = UI_TYPE_WINDOW,
	.freer = &ui_window_free,
	.getter = &ui_window_get,
	.values = g_accepted_menu
};

# define UI_ACCEPT_AMOUNT 4
static const t_ui_acceptable	g_acceptable[] =
{
	g_acceptable_button,
	g_acceptable_label,
	g_acceptable_menu,
	g_acceptable_window,
	NULL
};

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
		while (g_acceptable[i].values[++j])
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

t_vec4i	pos_arg_to_int_arr(char *str)
{
	t_vec4i	pos;
	char	*trimmed;	
	char	*final;
	char	**arr;
	int		i;

	trimmed = ft_strtrim(str);
	final = ft_strsub(trimmed, 1, ft_strlen(trimmed) - 2);
	arr = ft_strsplit(final, ',');
	i = -1;
	pos = vec4i(0, 0, 0, 0);
	while (arr[++i])
	{
		pos.v[i] = ft_atoi(arr[i]);
	}
	ft_arraydel(arr);
	return (pos);
}

void	bg_color_arg_to_int_arr(Uint32 *colors, char *str)
{
	char	*trimmed;	
	char	*final;
	char	**arr;
	char	**col_arr;
	char	*col_trimmed;
	int		i;

	trimmed = ft_strtrim(str);
	final = ft_strsub(trimmed, 1, ft_strlen(trimmed) - 2);
	arr = ft_strsplit(final, ',');
	i = -1;
	while (arr[++i])
	{
//		ft_printf("%s\n", arr[i]);
		col_trimmed = ft_strtrim(arr[i]);
		col_arr = ft_strsplitfirstoccurence(col_trimmed, ':');
//		ft_printf("[%#x]", strtoul(col_arr[1], NULL, 16));
//		ft_printf("== [%u]\n", strtoul(col_arr[1], NULL, 16));
		if (ft_strstr(col_arr[0], "default"))
			colors[UI_STATE_DEFAULT] = (unsigned int)strtoul(col_arr[1], NULL, 16);
		else if (ft_strstr(col_arr[0], "hover"))
			colors[UI_STATE_HOVER] = (unsigned int)strtoul(col_arr[1], NULL, 16);
		else if (ft_strstr(col_arr[0], "click"))
			colors[UI_STATE_CLICK] = (unsigned int)strtoul(col_arr[1], NULL, 16);
		ft_strdel(&col_trimmed);
		ft_arraydel(col_arr);
	}
	ft_strdel(&final);
	ft_strdel(&trimmed);
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

void	bg_image_arg_to_arr(char **images, char *str)
{
	char	*trimmed;
	char	*final;
	char	**arr;
	int		i;

	trimmed = ft_strtrim(str);
	final = ft_strsub(trimmed, 1, ft_strlen(trimmed) - 2);
	arr = ft_strsplit(final, ',');
	int	len;
	t_ui_key_value	*kv = make_key_value(arr, &len);
	i = -1;
	while (++i < len)
	{
		if (ft_strstr(kv[i].key, "default"))
			images[UI_STATE_DEFAULT] = trim_string(kv[i].value);
		else if (ft_strstr(kv[i].key, "hover"))
			images[UI_STATE_HOVER] = trim_string(kv[i].value);
		else if (ft_strstr(kv[i].key, "click"))
			images[UI_STATE_CLICK] = trim_string(kv[i].value);
	}
	free_key_value(kv, len);
	ft_strdel(&final);
	ft_strdel(&trimmed);
	ft_arraydel(arr);
}

void	ui_menu_get(t_ui_get *get)
{
	int			i;

	get->recipe->type = UI_TYPE_MENU;
	i = -1;
	while (++i < *get->len)
	{
		ft_printf("%s : %s\n", get->kv[i].key, get->kv[i].value);
		if (ft_strequ(get->kv[i].key, "pos"))
		{
			get->recipe->pos = pos_arg_to_int_arr(get->kv[i].value);
			get->recipe->pos_set = 1;
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

void	ui_window_get(t_ui_get *get)
{
	int			i;

	get->recipe->type = UI_TYPE_WINDOW;
	i = -1;
	while (++i < *get->len)
	{
		ft_printf("%s : %s\n", get->kv[i].key, get->kv[i].value);
		if (ft_strequ(get->kv[i].key, "pos"))
		{
			get->recipe->pos = pos_arg_to_int_arr(get->kv[i].value);
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
	int			i;

	get->recipe->type = UI_TYPE_LABEL;
	i = -1;
	while (++i < *get->len)
	{
		ft_printf("%s : %s\n", get->kv[i].key, get->kv[i].value);
		if (ft_strequ(get->kv[i].key, "pos"))
		{
			get->recipe->pos = pos_arg_to_int_arr(get->kv[i].value);
			get->recipe->pos_set = 1;
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
	}
}

void	ui_button_get(t_ui_get *get)
{
	int					i;

	get->recipe->type = UI_TYPE_BUTTON;
	i = -1;
	while (++i < *get->len)
	{
		if (ft_strequ(get->kv[i].key, "pos"))
		{
			get->recipe->pos = pos_arg_to_int_arr(get->kv[i].value);
			ft_printf("pos : %d %d %d %d\n", get->recipe->pos.x, get->recipe->pos.y, get->recipe->pos.w, get->recipe->pos.h);
			get->recipe->pos_set = 1;
		}
		else if (ft_strequ(get->kv[i].key, "bg_color"))
		{
			bg_color_arg_to_int_arr(get->recipe->bg_color, get->kv[i].value);
			ft_printf("bg_color : %#.8x %#.8x %#.8x\n",
				get->recipe->bg_color[UI_STATE_DEFAULT],
				get->recipe->bg_color[UI_STATE_HOVER],
				get->recipe->bg_color[UI_STATE_CLICK]);
			get->recipe->bg_color_set = 1;
		}
		else if (ft_strequ(get->kv[i].key, "bg_image"))
		{
			bg_image_arg_to_arr(get->recipe->bg_image, get->kv[i].value);
			ft_printf("bg_image : %s %s %s\n",
				get->recipe->bg_image[UI_STATE_DEFAULT],
				get->recipe->bg_image[UI_STATE_HOVER],
				get->recipe->bg_image[UI_STATE_CLICK]
				);
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

		values = ft_strsplit(inside, ';');
		int len;

		t_ui_key_value	*kv = make_key_value(values, &len);
		t_ui_recipe		*recipe = ft_memalloc(sizeof(t_ui_recipe));
		recipe->id = ft_strdup(var_name);
		ft_printf("Getting ID : %s\n", recipe->id);
		g_acceptable[i].getter(&(t_ui_get){&len, kv, recipe});
		add_to_list(&layout->recipes, recipe, sizeof(t_ui_recipe));
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

void	ui_layout_label_edit(t_ui_label *label, t_ui_recipe *recipe)
{
	if (recipe->pos_set)
		ui_label_pos_set(label, recipe->pos);
	if (recipe->title)
		ui_label_text_set(label, recipe->title);
	if (recipe->font_size_set)
		ui_label_size_set(label, recipe->font_size);
	if (recipe->font_color_set)
		ui_label_color_set(label, recipe->font_color);
	if (recipe->font_path_set)
		ui_label_font_set(label, recipe->font_path);
}

void	ui_layout_label_new(t_list **list, t_ui_window *win, t_ui_recipe *recipe, t_list *recipes)
{
	t_ui_label	*label;

	label = ft_memalloc(sizeof(t_ui_label));
	ui_label_new(win, label);
	ui_layout_label_edit(label, recipe);
	add_to_list(list, label, UI_TYPE_LABEL);
	(void)recipes;
}

void	ui_layout_button_new(t_list **list, t_ui_window *win, t_ui_recipe *recipe, t_list *recipes)
{
	t_ui_button	*button;
	t_ui_recipe	*child_recipe;

	button = ft_memalloc(sizeof(t_ui_button));
	ui_button_new(win, button);
	if (recipe->pos_set)
		ui_element_pos_set(&button->elem, recipe->pos);
	if (recipe->child_amount > 0)
	{
		child_recipe = get_recipe_by_id(recipes, recipe->children_ids[0]);
		ui_layout_label_edit(&button->label, child_recipe);
	}
	add_to_list(list, button, UI_TYPE_BUTTON);
}

void	ui_layout_menu_new(t_list **list, t_ui_window *win, t_ui_recipe *recipe, t_list *recipes)
{
	t_ui_menu	*menu;
	t_ui_recipe	*child_recipe;
	int			i;

	menu = ft_memalloc(sizeof(t_ui_menu));
	ui_menu_new(win, menu);
	if (recipe->pos_set)
		ui_element_pos_set(&menu->elem, recipe->pos);
	if (recipe->bg_color_set)
		ui_element_color_set(&menu->elem, UI_STATE_DEFAULT, recipe->bg_color[UI_STATE_DEFAULT]);
	i = -1;
	while (++i < recipe->child_amount)
	{
		child_recipe = get_recipe_by_id(recipes, recipe->children_ids[i]);
		if (child_recipe)
		{
			ui_layout_add_child(&menu->children, recipes, win, child_recipe);
			if (menu->children->content_size == UI_TYPE_BUTTON)
				ui_element_parent_set(&((t_ui_button *)menu->children->content)->elem, &menu->elem, UI_TYPE_ELEMENT, &menu->elem.show);
			else if (menu->children->content_size == UI_TYPE_LABEL)
				ft_putstr("we have found a label.\n");
			else
				ft_printf("[ui_layout_menu_new] Element of type %d is not supported.\n");
		}
	}
	add_to_list(list, menu, UI_TYPE_MENU);
}

void	ui_layout_add_child(t_list **list, t_list *recipes, t_ui_window *win, t_ui_recipe *recipe)
{
	if (recipe->type == UI_TYPE_LABEL)
		ui_layout_label_new(list, win, recipe, recipes);
	else if (recipe->type == UI_TYPE_BUTTON)
		ui_layout_button_new(list, win, recipe, recipes);
	else if (recipe->type == UI_TYPE_MENU)
		ui_layout_menu_new(list, win, recipe, recipes);
	else
		ft_printf("[ui_layout_add_child] ui_type : %d, not supported.\n", recipe->type);
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
			ui_layout_add_child(&layout->elements, layout->recipes, window, child_recipe);
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
		ft_printf("Window Not Found!\n");
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
