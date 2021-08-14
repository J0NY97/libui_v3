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

void	ui_read_button(char *id, FILE *fd)
{
	t_ui_element	elem;
	char			*line;
	char			**arr;
	size_t			len;
	ssize_t			nread;
	int				open_parentheses;

	open_parentheses = 1;
	line = NULL;
	nread = getline(&line, &len, fd);
	nread = getline(&line, &len, fd);
	while (open_parentheses > 0)
	{
		ft_printf("nread : %d\n", nread);
		arr = ft_strsplit(line, ' ');
		ft_printf("arr[0] : <%s>\n", arr[0]);
		if (ft_strchr(line, '{') || ft_strchr(line, '}'))
		{
			if (ft_strchr(line, '{'))
				open_parentheses++;
			if (ft_strchr(line, '}'))
				open_parentheses--;
		}
		if (ft_strstr(arr[0], "pos") == 0)
			ft_putstr("We have found pos.\n");
		else if (ft_strstr(arr[0], "bg_color") == 0)
			ft_putstr("We have found bg_color.\n");
		else
			ft_putstr("We have found presumably a variable of an element.\n");
		ft_strdel(&line);
		ft_arraydel(arr);
		nread = getline(&line, &len, fd);
	}
}

int	decide(char *str)
{
	if (ft_strequ(str, "Button"))
	{
		ft_putstr("We have found button.\n");
		return (UI_TYPE_BUTTON);
	}
	return (-1);
}

void	ui_load(char *ui_file_path)
{
	FILE	*fd;
	char	*line;
	char	**arr;
	size_t	len;
	ssize_t	nread;
	int		decision;

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
		arr = ft_strsplit(line, ' ');
		decision = decide(arr[0]);
		if (decision == -1)
		{
		}
		else if (decision == UI_TYPE_BUTTON)
			ui_read_button(arr[1], fd);
		ft_strdel(&line);
		ft_arraydel(arr);
		nread = getline(&line, &len, fd);
	}
	fclose(fd);
}
