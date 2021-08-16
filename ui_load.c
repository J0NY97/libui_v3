#include "libui_v3.h"

/*
 * Funt fact: you can only have flexible array in the end of a struct.
 * 	which actually makes sense if you think about it.
*/
typedef struct s_ui_acceptable
{
	char		*name;
	int			type;
	void		(*freer)(void *args);
	void		(*getter)(void *args);
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

typedef struct	s_ui_get
{
	char		*id;
	char		*inside;
	char		**values;
}				t_ui_get;

void	ui_label_get(void *label)
{
	(void)label;
}

void	ui_button_get(void *args) //(char *id, char *inside)
{
	char			**arr;
	int				i;
	int				j;
	t_ui_get		*butt_get;

	butt_get = args;
	ft_printf("id: %s\n", butt_get->id);
	i = -1;
	while (butt_get->values[++i])
	{
		j = -1;
		arr = ft_strsplit(butt_get->values[i], ':');
		while (g_accepted_button[++j])
		{
			if (ft_strstr(arr[0], g_accepted_button[j]))
				ft_printf("we have found %s arg.\n", g_accepted_button[j]);
		}
		if (ft_strequ(arr[0], "pos"))
		ft_arraydel(arr);
	}
}

void	decide(char *str, char *var_name, FILE *fd)
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
		g_acceptable[i].getter(&(t_ui_get){var_name, inside, values});
		ft_strdel(&inside);
		ft_arraydel(values);
	}
}

void	ui_load(char *ui_file_path)
{
	FILE	*fd;
	char	*line;
	char	*trimmed;
	char	**arr;
	size_t	len;
	ssize_t	nread;

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
			decide(arr[0], arr[1], fd);
		ft_strdel(&line);
		ft_strdel(&trimmed);
		ft_arraydel(arr);
		nread = getline(&line, &len, fd);
	}
	fclose(fd);
	ft_printf("Done reading : %s\n", ui_file_path);
}
