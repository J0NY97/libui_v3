#include "libui.h"

void	ui_layout_load_v2(t_ui_layout_v2 *layout, char *file)
{
	layout_read_file(layout, file);
	layout_split_elements(layout);
	layout_compile_elements(layout);
	layout_read_style(layout);
	layout_apply_style(layout);
}

char	*get_file_content(char *file)
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

void	layout_read_file(t_ui_layout_v2 *layout, char *file)
{
	memset(layout, 0, sizeof(t_ui_layout_v2));
	layout->layout_file = ft_strdup(file);
	layout->layout_file_content = get_file_content(file);
	ft_printf("%s", layout->layout_file_content);
}

void	layout_split_elements(t_ui_layout_v2 *layout)
{
	
}

/*
void	ui_element_compile_from_string(t_ui_window *win, char *str)
{
	int				i;
	int				j;
	int				opening;
	int				closing;
	t_ui_element	*elem;

	if (!str)
		return ;
	i = -1;
	j = -1;
	opening = 0;
	closing = 0;
	while (str[++i])
	{
		while (++j < UI_TYPE_AMOUNT)
			if (ft_strequ())
	}
}
*/

/*
 * Takes the layout_file_content and makes the elements with their children.
 * This find window and creates its elements.
*/
void	layout_compile_elements(t_ui_layout_v2 *layout)
{
	/*
	int			i;

	if (!layout->layout_file_content)
		return ;
	i = -1;
	while (layout->layout_file_content[++i])
	{
		
	}
	*/
}

void	layout_read_style(t_ui_layout_v2 *layout)
{
	
}

void	layout_apply_style(t_ui_layout_v2 *layout)
{
	
}
