#include "libui.h"

void	ui_radio_new(t_ui_window *win, t_ui_element *elem)
{
	t_ui_radio	*radio;

	ui_element_new(win, elem);
	ui_element_pos_set(elem, vec4(elem->pos.x, elem->pos.y, 0, 0));
	radio = ft_memalloc(sizeof(t_ui_radio));
	radio->elem = elem;
	elem->element = radio;
	elem->element_type = UI_TYPE_RADIO;
	radio->active = NULL;
}

void	ui_radio_event(t_ui_element *elem, SDL_Event e)
{
	t_ui_radio		*radio;
	t_list			*curr;

	if (!elem->show || !*elem->parent_show)
		return ;
	radio = elem->element;
	ui_list_radio_event(elem->children, &radio->active);
}

/*
 * Assumes everything in list is t_ui_element, and preferably t_ui_button type;
 * Return: if a new active 1 else 0;
*/
int	ui_list_radio_event(t_list *list, t_ui_element **active)
{
	t_ui_element	*elem;
	int				result;

	result = 0;
	while (list)
	{
		elem = list->content;
		elem->is_toggle = 0;
		if (elem->was_click)
		{
			if (*active != elem)
				result = 1;
			*active = elem;
		}
		else if (elem->is_hover)
			elem->state = UI_STATE_HOVER;
		else
			elem->state = UI_STATE_DEFAULT;
		list = list->next;
	}
	if (*active)
	{
		(*active)->state = UI_STATE_CLICK;
		(*active)->is_toggle = 1;
	}
	return (result);
}

/*
 * radio will only update its screen position so that
 * its children render correctly.
 * and ofc check if the parent of this elem was shown/renderer and other stuff that the renderer checks;
*/
int	ui_radio_render(t_ui_element *elem)
{
	if (!ui_element_render(elem))
		return (0);
	return (1);
}

void	ui_radio_free(void *elem, size_t size)
{
	(void)elem;
}

/*
 * Other functions
*/

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
	curr = elem->children;
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
