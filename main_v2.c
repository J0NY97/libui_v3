#include "libui.h"

int	main(void)
{
	t_ui_layout_v2	layout;

	ui_sdl_init();
	ui_layout_load_v2(&layout, "layout_v2.ui");
	return (0);
}
