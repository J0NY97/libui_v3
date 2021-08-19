#include "libui_v3.h"

void	ui_button_new(t_ui_window *win, t_ui_element *elem)
{
	ui_element_new(win, elem);
	elem->element = ft_memalloc(sizeof(t_ui_button));
	elem->element_type = UI_TYPE_BUTTON;
	((t_ui_button *)elem->element)->elem = elem;
	ui_element_color_set(elem, UI_STATE_DEFAULT, 0xff95d7ae);
	ui_element_color_set(elem, UI_STATE_HOVER, 0xff7bae7f);
	ui_element_color_set(elem, UI_STATE_CLICK, 0xff73956f);

	ui_label_new(win, &((t_ui_button *)elem->element)->label);
	ui_element_parent_set(&((t_ui_button *)elem->element)->label, elem, UI_TYPE_ELEMENT, &elem->show);
}

void	ui_button_event(t_ui_element *elem, SDL_Event e)
{
	if (!elem->show)
		return ;
	elem->is_hover = 0;
	if (point_in_rect(elem->win->mouse_pos, elem->screen_pos))
		elem->is_hover = 1;
	if (elem->is_hover && e.type == SDL_MOUSEBUTTONDOWN)
		elem->is_click = 1;
	if (elem->is_click && e.type == SDL_MOUSEBUTTONUP)
		elem->is_click = 0;
	if (elem->is_click)
		elem->state = UI_STATE_CLICK;
	else if (elem->is_hover)
		elem->state = UI_STATE_HOVER;
	else
		elem->state = UI_STATE_DEFAULT;
}

bool	ui_button(t_ui_element *elem)
{
	if (elem->is_click)
	{
		elem->is_click = 0;
		return (1);
	}
	return (0);
}

void	ui_button_render(t_ui_element *elem)
{
	if (!ui_element_render(elem))
		return ;
	ui_label_render(&((t_ui_button *)elem->element)->label);
	/*
	SDL_RenderTarget(button->elem.win->renderer, NULL);
	SDL_RenderCopy(button->elem.win->renderer, button->NULL, NULL);
	*/
}

/*
 * go figure.
*/
void	ui_button_free(void *button)
{
	(void)button;
}
