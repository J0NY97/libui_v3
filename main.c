#include "libui_v3.h"

int	main(void)
{
	ft_putstr("Hello world!\n");
	t_ui_window	win;
	t_ui_element elem1;
	t_ui_label label;
	t_ui_button button;

	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
	TTF_Init();

	// Window
	ui_window_new(&win, "test window", (t_vec4i){100, 100, 1280, 720});
	ui_texture_draw_border(win.renderer, win.texture, 2, 0xffff0000);

	// Element
	ui_element_new(&win, &elem1);
	ui_element_pos_set(&elem1, (t_vec4i){10, 50, 50, 20});
	ui_texture_fill_rect(elem1.win->renderer, elem1.textures[UI_STATE_DEFAULT], 0xffBF6900);

	// Label
	ui_label_new(&win, &label);

	// Button
	ui_button_new(&win, &button);

	SDL_Event	e;
	int	run = 1;
	while (run)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.key.type == SDL_KEYDOWN)
				if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
					run = 0;
			if (e.window.type == SDL_WINDOWEVENT)
				if (e.window.event == SDL_WINDOWEVENT_CLOSE)
					run = 0;
			ui_window_event(&win, e);
			ui_button_event(&button, e);
		}
		SDL_RenderClear(win.renderer);
		ui_window_render(&win);
//		ui_label_render(&label);
		ui_button_render(&button);
		ui_element_render(&elem1);
		SDL_RenderPresent(win.renderer);
	}

	return (0);
}
