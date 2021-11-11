#include "libui.h"

void	ui_checkbox_new(t_ui_window *win, t_ui_element *elem)
{
	t_ui_checkbox	*checkbox;

	ui_element_new(win, elem);
	checkbox = ft_memalloc(sizeof(t_ui_checkbox));
	elem->element = checkbox;
	elem->element_type = UI_TYPE_CHECKBOX;
}

/*
 * NOTE for DEV:
 * try to keep this element only,
 * because its used in the radio buttons.
 * (or atleast should at some point)
*/
void	ui_checkbox_event(t_ui_element *elem, SDL_Event e)
{
	elem->is_hover = ui_element_is_hover(elem); 
	if (elem->is_hover && elem->win->mouse_down_last_frame)
	{
		if (elem->is_toggle)
			ui_checkbox_toggle_off(elem);
		else
			ui_checkbox_toggle_on(elem);
	}
	if (elem->is_toggle)
		elem->is_click = 1;
	if (elem->is_click)
		elem->state = UI_STATE_CLICK;
	else if (elem->is_hover)
		elem->state = UI_STATE_HOVER;
	else
		elem->state = UI_STATE_DEFAULT;
}

int	ui_checkbox_render(t_ui_element *elem)
{
	if (!ui_element_render(elem))
		return (0);
}

void	ui_checkbox_free(void *elem)
{
	(void)elem;
}

/*
 * We don't need to check if the element_type is checkbox,
 * 	because we don't do anything to the checkbox element.
 * Might make this ui_element_toggle_on rather;
*/
void	ui_checkbox_toggle_on(t_ui_element *elem)
{
	elem->is_click = 1;
	elem->is_toggle = 1;
	elem->was_click = 1;
	elem->state = UI_STATE_CLICK;
}

void	ui_checkbox_toggle_off(t_ui_element *elem)
{
	elem->is_click = 0;
	elem->is_toggle = 0;
	if (elem->state == UI_STATE_CLICK)
		elem->state = UI_STATE_DEFAULT;
}
