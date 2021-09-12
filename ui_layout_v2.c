#include "libui.h"

void	ui_layout_load_v2(t_ui_layout_v2 *layout, char *file)
{
	layout_read_file(layout, file);
	layout_compile_elements(layout);
	layout_read_style(layout);
	layout_apply_style(layout);
}

void	layout_read_file(t_ui_layout_v2 *layout, char *file)
{
	FILE	*fd;

	fd = fopen(file, "r");
	if (!fd)
		return ;
	layout->layout_file = ft_strdup(file);
}

void	layout_compile_elements(t_ui_layout_v2 *layout)
{
	
}

void	layout_read_style(t_ui_layout_v2 *layout)
{
	
}

void	layout_apply_style(t_ui_layout_v2 *layout)
{
	
}
