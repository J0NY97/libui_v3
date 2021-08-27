#include "libui_v3.h"

int	main(void)
{
	ft_putstr("Hello world!\n");
	t_ui_layout		toolbox;
	t_ui_layout		guimp_toolbox;
	t_ui_window		win;
	t_ui_element	elem1;
	t_ui_element	label;
	t_ui_element	button;
	t_ui_element	button2;
	t_ui_element	button3;
	t_ui_element	menu;

	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
	TTF_Init();

	SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");

	ui_load(&toolbox, "toolbox.ui");
	ui_load(&guimp_toolbox, "guimp_toolbox.ui");

	// Edit guimp_toolbox elements in ways that are not possible in the ui file
	//
	//	testing
	t_ui_window		*test_window = ui_layout_get_window_by_id(&guimp_toolbox, "testing");
	//
	// Getting the color shower menu for later use.
	t_ui_element	*color_shower = ui_layout_get_element_by_id(&guimp_toolbox, "color_shower");
	t_ui_element	*hex_label = &((t_ui_input *)color_shower->element)->label;
	t_ui_slider		*color_r_slider = ui_layout_get_element_by_id(&guimp_toolbox, "r_slider")->element;
	t_ui_slider		*color_g_slider = ui_layout_get_element_by_id(&guimp_toolbox, "g_slider")->element;
	t_ui_slider		*color_b_slider = ui_layout_get_element_by_id(&guimp_toolbox, "b_slider")->element;
	t_ui_slider		*color_a_slider = ui_layout_get_element_by_id(&guimp_toolbox, "a_slider")->element;

	t_ui_element	*slider_elem = ui_layout_get_element_by_id(&guimp_toolbox, "r_slider");
	ui_slider_render(slider_elem);
	ui_texture_fill_rect(slider_elem->win->renderer, slider_elem->textures[UI_STATE_DEFAULT], 0xffff0000, vec4i(0, slider_elem->pos.h / 3, slider_elem->pos.w, slider_elem->pos.h / 3));

	slider_elem = ui_layout_get_element_by_id(&guimp_toolbox, "g_slider");
	ui_slider_render(slider_elem);
	ui_texture_fill_rect(slider_elem->win->renderer, slider_elem->textures[UI_STATE_DEFAULT], 0xff00ff00, vec4i(0, slider_elem->pos.h / 3, slider_elem->pos.w, slider_elem->pos.h / 3));

	slider_elem = ui_layout_get_element_by_id(&guimp_toolbox, "b_slider");
	ui_slider_render(slider_elem);
	ui_texture_fill_rect(slider_elem->win->renderer, slider_elem->textures[UI_STATE_DEFAULT], 0xff0000ff, vec4i(0, slider_elem->pos.h / 3, slider_elem->pos.w, slider_elem->pos.h / 3));

	slider_elem = ui_layout_get_element_by_id(&guimp_toolbox, "a_slider");
	// END Edit guimp_toolbox elements in ways that are not possible in the ui file

	// Window
	ui_window_new(&win, "test window", vec4(1920 - 800, 25, 800, 720));
	ui_window_flag_set(&win, UI_WINDOW_RESIZEABLE);
	ui_texture_fill(win.renderer, win.texture, 0xff404040);
	ui_texture_draw_border(win.renderer, win.texture, 2, 0xffff0000);

	// Element
	ui_element_new(&win, &elem1);
	ui_element_pos_set(&elem1, vec4(10, 50, 50, 20));
	ui_texture_fill(elem1.win->renderer, elem1.textures[UI_STATE_DEFAULT], 0xffBF6900);

	// Label
	ui_label_new(&win, &label);

	// Button
	ui_button_new(&win, &button);
	ui_element_pos_set(&button, vec4(10, 100, button.pos.w, button.pos.h));
	ui_label_text_set(&((t_ui_button *)button.element)->label, "Toggle Menu");
	ui_label_size_set(&((t_ui_button *)button.element)->label, 32);
	ui_label_color_set(&((t_ui_button *)button.element)->label, 0xff00ff00);

	ui_button_new(&win, &button2);
	ui_element_pos_set(&button2, vec4(10, 150, button.pos.w, button.pos.h));
	ui_label_text_set(&((t_ui_button *)button2.element)->label, "Toggle Button");
	ui_label_size_set(&((t_ui_button *)button2.element)->label, 32);
	ui_label_color_set(&((t_ui_button *)button2.element)->label, 0xff00ff00);

	ui_button_new(&win, &button3);
	ui_element_pos_set(&button3, vec4(10, 300, button.pos.w, button.pos.h));

	// Menu
	t_ui_element	menu_label0;
	t_ui_element	menu_button0;
	t_ui_element	menu_button1;
	t_ui_element	menu_menu0;
	t_ui_element	menu_dropdown0;
	t_ui_element	menu_slider0;
	ui_menu_new(&win, &menu);
	ui_element_pos_set(&menu, vec4(150, 100, 500, 500));
	ui_element_image_set_from_path(&menu, UI_STATE_DEFAULT, "images/grass_hill.jpg");

	ui_label_new(&win, &menu_label0);
	ui_label_text_set(&menu_label0, "Menu Label");
	ui_label_color_set(&menu_label0, 0xffff0000);
	ui_element_pos_set(&menu_label0, vec4(10, 10, 50, 20));

	ui_button_new(&win, &menu_button0);
	ui_element_pos_set(&menu_button0, vec4(10, 40, 50, 20));
	ui_element_color_set(&menu_button0, UI_STATE_DEFAULT, 0xffBF6900);
	ui_label_text_set(&((t_ui_button *)menu_button0.element)->label, "menu button");
	ui_label_color_set(&((t_ui_button *)menu_button0.element)->label, 0xffff0000);

	ui_button_new(&win, &menu_button1);
	ui_element_pos_set(&menu_button1, vec4(10, 70, 50, 20));
	ui_element_color_set(&menu_button1, UI_STATE_DEFAULT, 0xffBF6900);
	ui_label_text_set(&((t_ui_button *)menu_button1.element)->label, "Toggle menu0");
	ui_label_color_set(&((t_ui_button *)menu_button1.element)->label, 0xffff0000);

	ui_dropdown_new(&win, &menu_dropdown0);
	ui_element_pos_set(&menu_dropdown0, vec4(10, 100, 85, 20));

	ui_slider_new(&win, &menu_slider0);
	ui_element_pos_set(&menu_slider0, vec4(10, 150, 85, 20));

	t_ui_element	menu0_label;
	ui_label_new(&win, &menu0_label);
	ui_label_text_set(&menu0_label, "Menu0 Label");
	ui_label_color_set(&menu0_label, 0xffff0000);

	ui_menu_new(&win, &menu_menu0);
	ui_element_pos_set(&menu_menu0, vec4(0.5, 0.5, 0.1, 100));

	ui_menu_child_add(&menu_menu0, &menu0_label, UI_TYPE_ELEMENT);

	ui_menu_child_add(&menu, &menu_label0, UI_TYPE_ELEMENT);
	ui_menu_child_add(&menu, &menu_button0, UI_TYPE_ELEMENT);
	ui_menu_child_add(&menu, &menu_button1, UI_TYPE_ELEMENT);
	ui_menu_child_add(&menu, &menu_menu0, UI_TYPE_ELEMENT);
	ui_menu_child_add(&menu, &menu_dropdown0, UI_TYPE_ELEMENT);
	ui_menu_child_add(&menu, &menu_slider0, UI_TYPE_ELEMENT);

	// Dropdown
	t_ui_element	dropdown;

	ui_dropdown_new(&win, &dropdown);
	ui_element_pos_set(&dropdown, vec4(10, 190, 50, 20));

	// Input
	t_ui_element	input;
	ui_input_new(&win, &input);
	ui_element_pos_set(&input, vec4(150, 50, 50, 20));

	// Slider
	t_ui_element	slider;
	ui_slider_new(&win, &slider);
	ui_element_pos_set(&slider, vec4(150, 10, 50, 20));

	// Checkbox
	t_ui_element	checkbox0;
	ui_checkbox_new(&win, &checkbox0);
	ui_element_pos_set(&checkbox0, vec4(100, 10, 20, 20));

	ft_putstr("Inits done!\n");

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
			ui_button_event(&button3, e);
			ui_menu_event(&menu, e);
			ui_dropdown_event(&dropdown, e);
			ui_input_event(&input, e);
			ui_slider_event(&slider, e);
			ui_checkbox_event(&checkbox0, e);

			// Layout
			ui_layout_event(&toolbox, e);
			ui_layout_event(&guimp_toolbox, e);
		}

		// User Code
		if (ui_button(&button))
		{
			ft_printf("button is click.\n");
			menu.show = menu.show == 0;
		}
		if (ui_button(&button2))
			menu_button0.show = menu_button0.show == 0;
		if (ui_button(&menu_button0))
			ft_printf("clicketi clock\n");
		if (ui_button(&menu_button1))
			menu_menu0.show = menu_menu0.show == 0;
		ft_b_itoa(SDL_GetTicks(), temp);
		ui_label_text_set(&label, temp);

		// Layout user code
		// NOTE: only change label text of input if input state != 1
		Uint32 combined_slider_color = rgba_to_hex((t_rgba){.r = color_r_slider->value, .g = color_g_slider->value, .b = color_b_slider->value, .a = color_a_slider->value});
		ui_element_color_set(color_shower, UI_STATE_DEFAULT, combined_slider_color);
		if (!color_shower->is_click)
		{
			char temp[20];
			itoa(combined_slider_color, temp, 16);
			ui_label_text_set(hex_label, temp);
		}

		// Render
		SDL_RenderClear(win.renderer);
		ui_label_render(&label);
		ui_button_render(&button);
		ui_button_render(&button2);
		ui_button_render(&button3);
		ui_element_render(&elem1);
		ui_menu_render(&menu);
		ui_dropdown_render(&dropdown);
		ui_input_render(&input);
		ui_slider_render(&slider);
		ui_checkbox_render(&checkbox0);

		ui_window_render(&win);
		SDL_RenderPresent(win.renderer);

		// Layout
		ui_layout_render(&toolbox);
		ui_layout_render(&guimp_toolbox);
	}
	return (0);
}
