#include "libui.h"

int	main(void)
{
	int				run;
	SDL_Event		e;
	t_ui_layout_v2	layout;
	t_ui_window		*win_toolbox;
	t_ui_window		win2;
	t_ui_element	menu2;
	t_ui_element	*draw_button;

	ui_sdl_init();
	ui_layout_load_v2(&layout, "layout_v2.ui");
	win_toolbox = ui_list_get_window_by_id(layout.windows, "toolbox_window");

	draw_button = ui_list_get_element_by_id(layout.elements, "draw_button");

	ui_window_new(&win2, NULL, vec4(1920 - 200, 25, 200, 200));
	ui_window_pos_set(&win2, vec4(400, 25, 500, 500));
	ui_window_title_set(&win2, "testing window");
	ui_texture_fill(win2.renderer, win2.texture, 0xffff0000);

	ui_menu_new(&win2, &menu2);
	run = 1;
	while (run)
	{
		if (win_toolbox->wants_to_close)
			run = 0;
		while (SDL_PollEvent(&e))
		{
			ui_layout_event_v2(&layout, e);
			ui_window_event(&win2, e);
			ui_menu_event(&menu2, e);
		ft_printf("draw_button.. is_click : %d, was_click : %d\n", draw_button->is_click, draw_button->was_click);
		if (ui_button(draw_button))
			ft_printf("draw_button.. is_click : %d, was_click : %d\n", draw_button->is_click, draw_button->was_click);
		}
		ui_layout_render_v2(&layout);
		ui_menu_render(&menu2);
		ui_window_render(&win2);
		SDL_RenderPresent(win2.renderer); // IMPORTANT!!!!!
	}
	return (0);
}
