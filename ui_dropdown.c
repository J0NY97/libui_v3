#include "libui_v3.h"

/*
 * NOTE: the reason dropdown has element, is becuase of the parent stuff,
 * 	we dont actually need to show anything thats why the size of it is 0 x 0
*/
void	ui_dropdown_new(t_ui_window *win, t_ui_dropdown *drop)
{
	memset(drop, 0, sizeof(t_ui_dropdown));
	ui_element_new(win, &drop->elem);
	ui_element_color_set(&drop->elem, UI_STATE_DEFAULT, 0xffff0000);
	ui_element_pos_set(&drop->elem, vec4i(0, 0, 0, 0));

	ui_button_new(win, &drop->button);
	ui_label_text_set(&drop->button.label, "Dropdown");

	ui_menu_new(win, &drop->menu);
	ui_element_pos_set(&drop->menu.elem,
		vec4i(0, drop->button.elem.pos.h,
			drop->menu.elem.pos.w, drop->menu.elem.pos.h));

	drop->button.elem.parent = &drop->elem;
	drop->button.elem.parent_type = UI_TYPE_DROPDOWN;
	drop->button.elem.parent_show = &drop->elem.show;

	drop->menu.elem.parent = &drop->elem;
	drop->menu.elem.parent_type = UI_TYPE_DROPDOWN;
	drop->menu.elem.parent_show = &drop->elem.show;
}

void	ui_dropdown_event(t_ui_dropdown *drop, SDL_Event e)
{
	ui_button_event(&drop->button, e);
	if (ui_button(&drop->button))
		drop->menu.elem.show = drop->menu.elem.show == 0;
	if (drop->menu.elem.show)
		drop->button.elem.state = UI_STATE_CLICK;
	ui_menu_event(&drop->menu, e);
}

void	ui_dropdown_render(t_ui_dropdown *drop)
{
	ui_element_render(&drop->elem);
	ui_button_render(&drop->button);
	ui_menu_render(&drop->menu);
}
