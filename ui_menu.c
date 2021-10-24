#include "libui.h"

void	ui_menu_new(t_ui_window *win, t_ui_element *elem)
{
	t_ui_menu	*menu;

	ui_element_new(win, elem);
	elem->element = ft_memalloc(sizeof(t_ui_menu));
	elem->element_type = UI_TYPE_MENU;
	menu = elem->element;
	menu->elem = elem;
}

void	ui_menu_event(t_ui_element *elem, SDL_Event e)
{
	t_ui_menu		*menu;

	menu = elem->element;
	if (!menu->event_and_render_children)
		return ;
	ui_list_event(elem->children, e);
}

int	ui_menu_render(t_ui_element *elem)
{
	t_ui_menu		*menu;

	if (!ui_element_render(elem))
		return (0);
	menu = elem->element;
	if (!menu->event_and_render_children)
		return (1);
	ui_list_sort(elem->children);
	ui_list_render(elem->children);
	return (2);
}

void	ui_menu_free(void *menu)
{
	(void)menu;
}

// Gettesr

t_ui_menu	*ui_menu_get_menu(t_ui_element *elem)
{
	if (elem->element_type == UI_TYPE_MENU)
		return (elem->element);
	return (NULL);
}
