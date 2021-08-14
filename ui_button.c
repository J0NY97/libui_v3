#include "libui_v3.h"

void	ui_button_new(t_ui_window *win, t_ui_button *button)
{
	memset(button, 0, sizeof(t_ui_button));
	ui_element_new(win, &button->elem);
	ui_element_pos_set(&button->elem, (t_vec4i){10, 150, button->elem.pos.w, button->elem.pos.h});
	ui_texture_fill_rect(button->elem.win->renderer, button->elem.textures[UI_STATE_DEFAULT], 0xff95D7AE);
	ui_texture_fill_rect(button->elem.win->renderer, button->elem.textures[UI_STATE_HOVER], 0xff7BAE7F);
	ui_texture_fill_rect(button->elem.win->renderer, button->elem.textures[UI_STATE_CLICK], 0xff73956F);
	ui_label_new(win, &button->label);
	ui_label_pos_set(&button->label, (t_vec4i){button->elem.pos.x, button->elem.pos.y, button->label.pos.w, button->label.pos.h});
}

void	ui_button_event(t_ui_button *button, SDL_Event e)
{
	button->elem.is_hover = 0;
	if (point_in_rect(button->elem.win->mouse_pos, button->elem.pos))
		button->elem.is_hover = 1;
	if (button->elem.is_hover && e.type == SDL_MOUSEBUTTONDOWN)
		button->elem.is_click = 1;
	if (button->elem.is_click && e.type == SDL_MOUSEBUTTONUP)
		button->elem.is_click = 0;
	if (button->elem.is_click)
		button->elem.state = UI_STATE_CLICK;
	else if (button->elem.is_hover)
		button->elem.state = UI_STATE_HOVER;
	else
		button->elem.state = UI_STATE_DEFAULT;
}

bool	ui_button(t_ui_button *button)
{
	if (button->elem.is_click)
	{
		button->elem.is_click = 0;
		return (1);
	}
	return (0);
}

void	ui_button_render(t_ui_button *button)
{
	ui_element_render(&button->elem);
	ui_label_render(&button->label);
	/*
	SDL_RenderTarget(button->elem.win->renderer, NULL);
	SDL_RenderCopy(button->elem.win->renderer, button->NULL, NULL);
	*/
}
