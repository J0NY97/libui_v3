#include "libui.h"

void	ui_radio_new(t_ui_window *win, t_ui_element *elem)
{
	t_ui_radio	*radio;

	ui_element_new(win, elem);
	radio = ft_memalloc(sizeof(t_ui_radio));
	radio->elem = elem;
	elem->element = radio;
	elem->element_type = UI_TYPE_RADIO;
	radio->buttons = NULL;
	radio->active = NULL;
}

void	ui_radio_event(t_ui_element *elem, SDL_Event e)
{
	t_ui_element	*child;
	t_ui_radio		*radio;
	t_list			*curr;

	if (!elem->show || !*elem->parent_show)
		return ;
	radio = elem->element;
	curr = radio->buttons;
	while (curr)
	{
		child = curr->content;
		ui_button_event(child, e);
		if (child->is_click)
			radio->active = child;
		else if (child->is_hover)
			child->state = UI_STATE_HOVER;
		else
			child->state = UI_STATE_DEFAULT;
		curr = curr->next;
	}
	if (radio->active)
		radio->active->state = UI_STATE_CLICK;
}

int	ui_radio_render(t_ui_element *elem)
{
	t_ui_radio	*radio;
	t_list		*curr;

	if (!ui_element_render(elem))
		return (0);
	radio = elem->element;
	curr = radio->buttons;
	while (curr)
	{
		ui_button_render(curr->content);
		curr = curr->next;
	}
	return (1);
}

void	ui_radio_free(void *elem)
{
	(void)elem;
}

/*
 * Other functions
*/

/*
 * Adds button to the button list.
 * Note: we make elem the parent of child.
*/
void	ui_radio_add(t_ui_element *elem, t_ui_element *child)
{
	t_ui_radio	*radio;

	radio = elem->element;
	ui_element_parent_set(child, elem, UI_TYPE_ELEMENT);
	add_to_list(&radio->buttons, child, UI_TYPE_ELEMENT);
}

/*
 * if 'toggle_this' is in radio button list it will toggle it on,
 * and untoggle all the other buttons;
*/
void	ui_radio_button_toggle_on(t_ui_element *elem, t_ui_element *toggle_this)
{
	t_ui_element	*child;
	t_ui_radio		*radio;
	t_list			*curr;

	radio = elem->element;
	curr = radio->buttons;
	while (curr)
	{
		child = curr->content;
		if (toggle_this == child)
			radio->active = toggle_this;
		else
			child->state = UI_STATE_DEFAULT;
		curr = curr->next;
	}
	if (radio->active)
		radio->active->state = UI_STATE_CLICK;
}
