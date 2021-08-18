#include "libui_v3.h"

/*
 * Recipes.
*/

typedef struct s_ui_window_recipe
{
	char				*id;
	t_vec4i				pos;	
	char				*title;
	Uint32				bg_color;
}						t_ui_window_recipe;

typedef struct s_ui_label_recipe
{
	char				*id;
	t_vec4i				pos;	
	char				*font_path;
	char				*font_size;
	Uint32				font_color;
}						t_ui_label_recipe;

typedef struct s_ui_button_recipe
{
	char				*id;
	t_vec4i				pos;	
	Uint32				bg_color[UI_STATE_AMOUNT];
	char				*bg_image[UI_STATE_AMOUNT];
	t_ui_label_recipe	label;
}						t_ui_button_recipe;

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
	char		*id;
	char		*inside;
	char		**values;
}				t_ui_get;

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

void	bg_color_arg_to_int_arr(Uint32 **colors, char *str)
{
	char	*trimmed;	
	char	*final;
	char	**arr;
	char	**col_arr;
	char	*col_trimmed;
	int		i;
	int		j;

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
	char	**img_arr;
	char	*img_trimmed;
	char	*trimtram;
	char	*subbed;
	int		i;
	int		j;

	trimmed = ft_strtrim(str);
	final = ft_strsub(trimmed, 1, ft_strlen(trimmed) - 2);
	arr = ft_strsplit(final, ',');
	i = -1;
	while (arr[++i])
	{
//		ft_printf("%s\n", arr[i]);
		img_trimmed = ft_strtrim(arr[i]);
//		ft_printf("img_trimmed : %s\n", img_trimmed);
		img_arr = ft_strsplitfirstoccurence(img_trimmed, ':');
//		ft_printf("img_arr[0] : <%s>\n", img_arr[0]);
//		ft_printf("img_arr[1] : <%s>\n", img_arr[1]);
		trimtram = ft_strtrim(img_arr[1]);
//		ft_printf("trimtram : <%s>, len : %d\n", trimtram, ft_strlen(trimtram));
		subbed = ft_strsub(trimtram, 1, ft_strlen(trimtram) - 2); 
//		ft_printf("subbed : <%s>\n", subbed);
		if (ft_strstr(img_arr[0], "default"))
			images[UI_STATE_DEFAULT] = subbed;
		else if (ft_strstr(img_arr[0], "hover"))
			images[UI_STATE_HOVER] = subbed;
		else if (ft_strstr(img_arr[0], "click"))
			images[UI_STATE_CLICK] = subbed; 
		ft_strdel(&subbed);
		ft_strdel(&trimtram);
		ft_strdel(&img_trimmed);
		ft_arraydel(img_arr);
	}
	ft_strdel(&final);
	ft_strdel(&trimmed);
	ft_arraydel(arr);
}

typedef struct s_ui_key_value
{
	char		*key;
	char		*value;
}				t_ui_key_value;

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

void	ui_window_get(t_ui_layout *layout, void *args)
{
	t_ui_window_recipe	*recipe;
	t_ui_get			*get;
	t_ui_key_value		*kv;
	int					i;
	int					len;

	recipe = ft_memalloc(sizeof(t_ui_window_recipe));
	get = args;	
	ft_printf("Window ID : %s\n", get->id);
	recipe->id = ft_strdup(get->id);
	i = -1;
	kv = make_key_value(get->values, &len);
	while (++i < len)
	{
		if (ft_strequ(kv[i].key, "pos"))
		{
			recipe->pos = pos_arg_to_int_arr(kv[i].value);
		}
		else if (ft_strequ(kv[i].key, "title"))
		{
			recipe->title = trim_string(kv[i].value);
		}
		else if (ft_strequ(kv[i].key, "bg_color"))
		{
			recipe->bg_color = strtoul(kv[i].value, NULL, 16);
		}
	}
	add_to_list(&layout->recipes, recipe, sizeof(t_ui_window_recipe));
}

void	ui_label_get(t_ui_layout *layout, void *args)
{
	t_ui_label_recipe	*recipe;
	t_ui_get			*get;
	t_ui_key_value		*kv;
	char				**arr;
	char				*trimmed;
	int					i;

	recipe = ft_memalloc(sizeof(t_ui_label_recipe));
	get = args;	
	ft_printf("Label ID : %s\n", get->id);
	recipe->id = ft_strdup(get->id);
	i = -1;
	int	len;
	kv = make_key_value(get->values, &len);
	while (++i < len)
	{
		ft_printf("%s : %s\n", kv[i].key, kv[i].value);
		if (ft_strequ(kv[i].key, "pos"))
		{
			recipe->pos = pos_arg_to_int_arr(kv[i].value);
		}
		else if (ft_strequ(kv[i].key, "font_path"))
		{
			recipe->font_path = trim_string(kv[i].value);
		}
		else if (ft_strequ(kv[i].key, "font_size"))
		{
			recipe->font_size = ft_atoi(kv[i].value);
		}
		else if (ft_strequ(kv[i].key, "font_color"))
		{
			recipe->font_color = strtoul(kv[i].value, NULL, 16);
		}
	}
	add_to_list(&layout->recipes, recipe, sizeof(t_ui_label_recipe));
}

void	ui_button_get(t_ui_layout *layout, void *args)
{
	t_ui_button_recipe	*recipe;
	int					i;
	t_ui_get			*get;
	t_ui_key_value		*kv;

	recipe = ft_memalloc(sizeof(t_ui_button_recipe));
	get = args;
	ft_printf("id: %s\n", get->id);
	recipe->id = ft_strdup(get->id);
	i = -1;
	int	len;
	kv = make_key_value(get->values, &len);
	while (++i < len)
	{
		if (ft_strequ(kv[i].key, "pos"))
		{
			recipe->pos = pos_arg_to_int_arr(kv[i].value);
			ft_printf("pos : %d %d %d %d\n", recipe->pos.x, recipe->pos.y, recipe->pos.w, recipe->pos.h);
		}
		else if (ft_strequ(kv[i].key, "bg_color"))
		{
			bg_color_arg_to_int_arr(&recipe->bg_color, kv[i].value);
			ft_printf("bg_color : %#.8x %#.8x %#.8x\n",
				recipe->bg_color[UI_STATE_DEFAULT],
				recipe->bg_color[UI_STATE_HOVER],
				recipe->bg_color[UI_STATE_CLICK]);
		}
		else if (ft_strequ(kv[i].key, "bg_image"))
		{
			bg_image_arg_to_arr(recipe->bg_image, kv[i].value);
			ft_printf("bg_image : %s %s %s\n",
				recipe->bg_image[UI_STATE_DEFAULT],
				recipe->bg_image[UI_STATE_HOVER],
				recipe->bg_image[UI_STATE_CLICK]
				);
		}
	}
	add_to_list(&layout->recipes, recipe, sizeof(t_ui_button_recipe));
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
		g_acceptable[i].getter(layout, &(t_ui_get){var_name, inside, values});
		ft_strdel(&inside);
		ft_arraydel(values);
//		free_ui_get();
	}
}

void	ui_layout_window_new(t_ui_layout *layout, t_ui_window_recipe *recipe)
{
	t_ui_window	*window;

	window = malloc(sizeof(t_ui_window));
	ui_window_new(window, recipe->title, recipe->pos);
	window->id = ft_strdup(recipe->id);
	ui_texture_fill(window->renderer, window->texture, recipe->bg_color);
	add_to_list(&layout->windows, window, sizeof(t_ui_window));
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
	t_ui_window_recipe	*recipe;

	curr = layout->recipes;
	recipe = NULL;
	while (curr)
	{
		if (curr->content_size == sizeof(t_ui_window_recipe))
		{
			recipe = curr->content;
			ui_layout_window_new(layout, recipe);
						/* find all the elements and make them
			while ()
			{}
			*/
		}
		curr = curr->next;
	}
	if (recipe)
		ft_printf("Window Found : %s\n", recipe->id);
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
