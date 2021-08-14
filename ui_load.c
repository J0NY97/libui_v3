#include "libui_v3.h"

static const char *g_accepted_button[] = {
	"Button",
	"pos",
	"bg_color",
	"something",
	NULL
};

static const char *g_accepted_window[] = {
	"Window",
	"pos",
	NULL
};

static const char *g_accepted_menu[] = {
	"Menu",
	"pos",
	NULL
};

static const char *g_accepted_label[] = {
	"Label",
	"pos",
	"font_path",
	"font_size",
	"font_color",
	NULL
};

/*
 * Everything in Menu/Window that are not in the accepted,
 * are considered as already made elements in the ui file,
 * and will be tried to get_element_by_id from a list of elements.
 *
 * If Button has something that is not in the accepted,
 * it will assume its a label, and try to find that label id
 * from the list of elements.
*/
static const char **g_accepted[] = {
	g_accepted_button,
	g_accepted_window,
	g_accepted_menu,
	g_accepted_label,
	NULL
};

void	ui_print_accepted(void)
{
	int	i;
	int	j;

	i = -1;
	while (g_accepted[++i])
	{
		j = -1;
		ft_printf("\n[%s]\n{\n", g_accepted[i][++j]);
		while (g_accepted[i][++j])
		{
			ft_printf(" %s\n", g_accepted[i][j]);
		}
		ft_printf("}\n");
	}
}

void	ui_read_button(char *id, char *inside)
{
	t_ui_element	elem;
	char			**values;
	char			**arr;
	int				i;

	values = ft_strsplit(inside, ';');
	i = -1;
	while (values[++i])
	{
		arr = ft_strsplit(values[i], ':');
		if (ft_strstr(arr[0], "pos"))
			ft_putstr("we have found pos arg.\n");
		else if (ft_strstr(arr[0], "bg_color"))
			ft_putstr("we have found bg_color arg.\n");
		else
			ft_putstr("we have found presumably element variable arg.\n");
		ft_arraydel(arr);
	}
	ft_arraydel(values);
}

int	decide(char *str, char **inside, FILE *fd)
{
	char	*line;
	size_t	len;
	ssize_t	nread;
	int		result;
	char	*trimmed;
	int		open_parentheses;

	result = -1;
	if (ft_strequ(str, "Button"))
	{
		ft_putstr("We have found button.\n");
		result = UI_TYPE_BUTTON;
	}
	if (result != -1)
	{
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
			ft_stradd(inside, trimmed);
			ft_strdel(&trimmed);
			ft_strdel(&line);
			if (open_parentheses == 0)
				break ;
			nread = getline(&line, &len, fd);
		}
		ft_strdel(&line);
		trimmed = ft_strsub(*inside, 1, ft_strlen(*inside) - 2);
		ft_strdel(inside);
		*inside = ft_strdup(trimmed);
		ft_strdel(&trimmed);
	}
	return (result);
}

void	ui_load(char *ui_file_path)
{
	FILE	*fd;
	char	*line;
	char	**arr;
	size_t	len;
	ssize_t	nread;
	int		decision;
	char	*inside;

	fd = fopen(ui_file_path, "r");
	if (!fd)
	{
		ft_printf("[ui_load] Couldnt open ui file : %s\n", ui_file_path);
		return;
	}
	else
		ft_printf("[ui_load] Could open ui file : %s\n", ui_file_path);
	line = NULL;
	inside = NULL;
	nread = getline(&line, &len, fd);
	while (nread != -1)
	{
		ft_printf("nread : %d\n", nread);
		arr = ft_strsplit(line, ' ');
		decision = decide(arr[0], &inside, fd);
		if (decision == -1)
		{
		}
		else if (decision == UI_TYPE_BUTTON)
			ui_read_button(arr[1], inside);
		ft_strdel(&line);
		ft_strdel(&inside);
		ft_arraydel(arr);
		nread = getline(&line, &len, fd);
	}
	fclose(fd);
}
