#include "libui_v3.h"

int	main(void)
{
	ft_putstr("Hello world!\n");
	t_ui_window	win;
	t_ui_element elem1;
	t_ui_label label;
	t_ui_button button;
	t_ui_button button2;
	t_ui_menu	menu;

	ui_print_accepted();
	ui_load("layout.ui");

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
	ui_element_pos_set(&button.elem, vec4i(10, 100, 50, 20));
	ui_label_text_set(&button.label, "Toggle Menu");
	ui_label_size_set(&button.label, 32);
	ui_label_color_set(&button.label, 0xff00ff00);

	ui_button_new(&win, &button2);
	ui_element_pos_set(&button2.elem, vec4i(10, 150, 50, 20));
	ui_label_text_set(&button2.label, "Toggle Button");
	ui_label_size_set(&button2.label, 32);
	ui_label_color_set(&button2.label, 0xff00ff00);

	// Menu
	t_ui_label		menu_label0;
	t_ui_button		menu_button0;
	t_ui_button		menu_button1;
	ui_menu_new(&win, &menu);
	ui_element_pos_set(&menu.elem, (t_vec4i){150, 100, 500, 500});
	ui_element_image_set_from_path(&menu.elem, UI_STATE_DEFAULT, "images/grass_hill.jpg");

	ui_label_new(&win, &menu_label0);
	ui_label_text_set(&menu_label0, "Menu Label");
	ui_label_color_set(&menu_label0, 0xffff0000);
	ui_label_pos_set(&menu_label0, vec4i(10, 10, 50, 20));

	ui_button_new(&win, &menu_button0);
	ui_element_pos_set(&menu_button0.elem, vec4i(10, 40, 50, 20));
	ui_element_color_set(&menu_button0.elem, UI_STATE_DEFAULT, 0xffBF6900);
	ui_label_text_set(&menu_button0.label, "menu button");
	ui_label_color_set(&menu_button0.label, 0xffff0000);

	ui_button_new(&win, &menu_button1);
	ui_element_pos_set(&menu_button1.elem, vec4i(10, 70, 50, 20));
	ui_element_color_set(&menu_button1.elem, UI_STATE_DEFAULT, 0xffBF6900);
	ui_label_text_set(&menu_button1.label, "another one");
	ui_label_color_set(&menu_button1.label, 0xffff0000);

	ui_menu_child_add(&menu, &menu_label0, UI_TYPE_LABEL);
	ui_menu_child_add(&menu, &menu_button0, UI_TYPE_BUTTON);
	ui_menu_child_add(&menu, &menu_button1, UI_TYPE_BUTTON);

	SDL_Event	e;
	int	run = 1;
	char temp[20];
	while (run)
	{
		// Input
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
			ui_button_event(&button2, e);
			ui_menu_event(&menu, e);
		}

		// User Code
		if (ui_button(&button))
		{
			ft_printf("button is click.\n");
			menu.elem.show = menu.elem.show == 0;
		}
		if (ui_button(&button2))
			menu_button0.elem.show = menu_button0.elem.show == 0;
		if (ui_button(&menu_button0))
			ft_printf("clicketi clock\n");
		ft_b_itoa(SDL_GetTicks(), temp);
		ui_label_text_set(&label, temp);

		// Render
		SDL_RenderClear(win.renderer);
		ui_window_render(&win);
		ui_label_render(&label);
		ui_button_render(&button);
		ui_button_render(&button2);
		ui_element_render(&elem1);
		ui_menu_render(&menu);
		SDL_RenderPresent(win.renderer);
	}
	return (0);
}
