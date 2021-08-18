#include "libui_v3.h"

/*
 * int		type;		one of enum e_element_type
*/
typedef struct s_ui_recipe
{
	char				*id;
	int					type;

	t_vec4i				pos;	
	bool				pos_set;

	char				*title;

	char				*font_path;

	Uint32				font_size;
	bool				font_size_set;

	Uint32				font_color;
	bool				font_color_set;

	Uint32				bg_color[UI_STATE_AMOUNT];
	char				*bg_image[UI_STATE_AMOUNT];

	char				**children_ids;
	int					child_amount;
	t_list				*recipes;
}						t_ui_recipe;

/*
 * Fun fact: you can only have flexible array in the end of a struct.
 * 	which actually makes sense if you think about it.
*/
typedef struct s_ui_acceptable
{
	char		*name;
	int			type;
	void		(*freer)(void *args);
	void		(*getter)(t_ui_layout *layout, void *args);
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

typedef struct s_ui_key_value
{
	char		*key;
	char		*value;
}				t_ui_key_value;
/*
 * arg : value
 *
 * args is the argument of that you want to change in the element,
 * t.ex pos, bg_color, bg_image.
 *
 * values is the value that comes after the args,
 * t.ex bg_color : 0xff000000, the '0xff000000' is the value.
*/
typedef struct	s_ui_get
{
	char			*id;
	char			*inside;
	int				*len; // len of kv
	t_ui_key_value	*kv;
}					t_ui_get;

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

/*
 * Give in str usually == 'arg : "string"'
 * Give out arr[0] = arg
 * 			arr[1] = string
*/
char	**key_value_get_string(char *str)
{
	char	**arr;	
	char	*trim;

	if (!str)
		return (NULL);
	arr = ft_strsplitfirstoccurence(str, ':');
	if (!arr[0] || !arr[1])
		return (NULL);
	trim = ft_strtrim(arr[0]);
	ft_strdel(&arr[0]);
	arr[0] = ft_strdup(trim);
	ft_strdel(&trim);

	trim = trim_string(arr[1]);
	ft_strdel(&arr[1]);
	arr[1] = ft_strdup(trim);
	ft_strdel(&trim);
	return (arr);
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

void	ui_window_get(t_ui_layout *layout, void *args)
{
	t_ui_recipe	*recipe;
	t_ui_get	*get;
	int			i;

	recipe = ft_memalloc(sizeof(t_ui_recipe));
	recipe->type = UI_TYPE_WINDOW;
	get = args;	
	ft_printf("Window ID : %s\n", get->id);
	recipe->id = ft_strdup(get->id);
	i = -1;
	while (++i < *get->len)
	{
		ft_printf("%s : %s\n", get->kv[i].key, get->kv[i].value);
		if (ft_strequ(get->kv[i].key, "pos"))
		{
			recipe->pos = pos_arg_to_int_arr(get->kv[i].value);
		}
		else if (ft_strequ(get->kv[i].key, "title"))
		{
			recipe->title = trim_string(get->kv[i].value);
		}
		else if (ft_strequ(get->kv[i].key, "bg_color"))
		{
			recipe->bg_color[UI_STATE_DEFAULT] = strtoul(get->kv[i].value, NULL, 16);
		}
		else // should be variables
		{
			recipe->children_ids = realloc(recipe->children_ids, sizeof(char *) * (++recipe->child_amount + 1));
			recipe->children_ids[recipe->child_amount - 1] = ft_strdup(get->kv[i].key);
		}
	}
	recipe->children_ids[recipe->child_amount] = NULL;
	add_to_list(&layout->recipes, recipe, sizeof(t_ui_recipe));
}

void	ui_label_get(t_ui_layout *layout, void *args)
{
	t_ui_recipe	*recipe;
	t_ui_get	*get;
	int			i;

	recipe = ft_memalloc(sizeof(t_ui_recipe));
	recipe->type = UI_TYPE_LABEL;
	get = args;	
	ft_printf("Label ID : %s\n", get->id);
	recipe->id = ft_strdup(get->id);
	i = -1;
	while (++i < *get->len)
	{
		ft_printf("%s : %s\n", get->kv[i].key, get->kv[i].value);
		if (ft_strequ(get->kv[i].key, "pos"))
		{
			recipe->pos = pos_arg_to_int_arr(get->kv[i].value);
			recipe->pos_set = 1;
		}
		else if (ft_strequ(get->kv[i].key, "title"))
		{
			recipe->title = trim_string(get->kv[i].value);
		}
		else if (ft_strequ(get->kv[i].key, "font_path"))
		{
			recipe->font_path = trim_string(get->kv[i].value);
		}
		else if (ft_strequ(get->kv[i].key, "font_size"))
		{
			recipe->font_size = ft_atoi(get->kv[i].value);
			recipe->font_size_set = 1;
		}
		else if (ft_strequ(get->kv[i].key, "font_color"))
		{
			recipe->font_color = strtoul(get->kv[i].value, NULL, 16);
			recipe->font_color_set = 1;
		}
	}
	add_to_list(&layout->recipes, recipe, sizeof(t_ui_recipe));
}

void	ui_button_get(t_ui_layout *layout, void *args)
{
	t_ui_recipe	*recipe;
	int					i;
	t_ui_get			*get;

	recipe = ft_memalloc(sizeof(t_ui_recipe));
	recipe->type = UI_TYPE_BUTTON;
	get = args;
	ft_printf("id: %s\n", get->id);
	recipe->id = ft_strdup(get->id);
	i = -1;
	while (++i < *get->len)
	{
		if (ft_strequ(get->kv[i].key, "pos"))
		{
			recipe->pos = pos_arg_to_int_arr(get->kv[i].value);
			ft_printf("pos : %d %d %d %d\n", recipe->pos.x, recipe->pos.y, recipe->pos.w, recipe->pos.h);
		}
		else if (ft_strequ(get->kv[i].key, "bg_color"))
		{
			bg_color_arg_to_int_arr(recipe->bg_color, get->kv[i].value);
			ft_printf("bg_color : %#.8x %#.8x %#.8x\n",
				recipe->bg_color[UI_STATE_DEFAULT],
				recipe->bg_color[UI_STATE_HOVER],
				recipe->bg_color[UI_STATE_CLICK]);
		}
		else if (ft_strequ(get->kv[i].key, "bg_image"))
		{
			bg_image_arg_to_arr(recipe->bg_image, get->kv[i].value);
			ft_printf("bg_image : %s %s %s\n",
				recipe->bg_image[UI_STATE_DEFAULT],
				recipe->bg_image[UI_STATE_HOVER],
				recipe->bg_image[UI_STATE_CLICK]
				);
		}
	}
	add_to_list(&layout->recipes, recipe, sizeof(t_ui_recipe));
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
		g_acceptable[i].getter(layout, &(t_ui_get){var_name, inside, &len, kv});
		free_key_value(kv, len);
		ft_strdel(&inside);
		ft_arraydel(values);
//		free_ui_get();
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

void	ui_layout_label_new(t_ui_layout *layout, t_ui_window *win, t_ui_recipe *recipe)
{
	t_ui_label	*label;

	label = ft_memalloc(sizeof(t_ui_label));
	ui_label_new(win, label);
	if (recipe->pos_set)
		ui_label_pos_set(label, recipe->pos);
	if (recipe->title)
		ui_label_text_set(label, recipe->title);
	if (recipe->font_size_set)
		ui_label_size_set(label, recipe->font_size);
	if (recipe->font_color_set)
		ui_label_color_set(label, recipe->font_color);
	add_to_list(&layout->elements, label, sizeof(t_ui_label));
}

void	ui_layout_element_new(t_ui_layout *layout, t_ui_window *win, t_ui_recipe *recipe)
{
	if (recipe->type == UI_TYPE_LABEL)
		ui_layout_label_new(layout, win, recipe);
	else
		ft_printf("[ui_layout_element_new] ui_type : %d, not supported.\n", recipe->type);
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
	add_to_list(&layout->windows, window, sizeof(t_ui_window));

	i = -1;
	ft_printf("Child Amount : %d\n", recipe->child_amount);
	while (++i < recipe->child_amount)
	{
		ft_printf("#%d %s\n", i, recipe->children_ids[i]);
		child_recipe = get_recipe_by_id(layout->recipes, recipe->children_ids[i]);
		if (child_recipe)
		{
			ft_printf("[ui_layout_window_new] Trying to make: %s\n", child_recipe->id);
			ui_layout_element_new(layout, window, child_recipe);
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
