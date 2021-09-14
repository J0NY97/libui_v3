#include "libui.h"

int	main(void)
{
	int				run;
	SDL_Event		e;
	t_ui_layout_v2	layout;
	t_ui_window		*win;
	t_ui_window		win2;
	t_ui_element	menu2;

	ui_sdl_init();
	ui_layout_load_v2(&layout, "layout_v2.ui");
	win = ui_list_get_window_by_id(layout.windows, "win0");
	ui_window_new(&win2, NULL, vec4(1920 - 200, 25, 200, 200));
	ui_texture_fill(win2.renderer, win2.texture, 0xffff0000);
	ui_menu_new(&win2, &menu2);
	run = 1;
	while (run)
	{
		if (win->wants_to_close)
			run = 0;
		while (SDL_PollEvent(&e))
		{
			ui_layout_event_v2(&layout, e);
			ui_window_event(&win2, e);
			ui_menu_event(&menu2, e);
		}
		ui_layout_render_v2(&layout);
		ui_menu_render(&menu2);
		ui_window_render(&win2);
		SDL_RenderPresent(win2.renderer); // IMPORTANT!!!!!
	}
	return (0);
}
