#include "libui_v3.h"

void	ui_button_new(t_ui_window *win, t_ui_element *elem)
{
	t_ui_button	*button;

	ui_element_new(win, elem);
	elem->element = ft_memalloc(sizeof(t_ui_button));
	elem->element_type = UI_TYPE_BUTTON;
	button = elem->element;
	button->elem = elem;
	ui_element_color_set(elem, UI_STATE_DEFAULT, 0xff95d7ae);
	ui_element_color_set(elem, UI_STATE_HOVER, 0xff7bae7f);
	ui_element_color_set(elem, UI_STATE_CLICK, 0xff73956f);

	ui_label_new(win, &button->label);
	ui_label_text_align(&button->label, UI_TEXT_ALIGN_CENTER);
	ui_element_parent_set(&button->label, elem, UI_TYPE_ELEMENT, &elem->show);
}

void	ui_button_event(t_ui_element *elem, SDL_Event e)
{
	if (!elem->show)
		return ;
	elem->is_hover = ui_element_is_hover(elem);
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

int	ui_button_render(t_ui_element *elem)
{
	t_ui_button	*button;

	if (!ui_element_render(elem))
		return (0);
	button = elem->element;
	ui_label_render(&button->label);
	return (1);
}

/*
 * go figure.
*/
void	ui_button_free(void *button)
{
	(void)button;
}
