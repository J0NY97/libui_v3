#include "libui.h"

void	ui_menu_new(t_ui_window *win, t_ui_element *elem)
{
	ui_element_new(win, elem);
	elem->element = ft_memalloc(sizeof(t_ui_menu));
	elem->element_type = UI_TYPE_MENU;
	((t_ui_menu *)elem->element)->children = NULL;
	((t_ui_menu *)elem->element)->elem = elem;
}

void	ui_menu_add(t_ui_element *elem, t_ui_element *child)
{
	t_ui_menu	*menu;

	menu = elem->element;
	ui_element_parent_set(child, elem, UI_TYPE_ELEMENT);
	add_to_list(&menu->children, child, UI_TYPE_ELEMENT);
}

void	ui_menu_event(t_ui_element *elem, SDL_Event e)
{
	t_list			*curr;
	t_ui_element	*child;
	t_ui_menu		*menu;

	menu = elem->element;
	curr = menu->children;
	while (curr)
	{
		child = curr->content;
		if (curr->content_size == UI_TYPE_ELEMENT)
		{
			if (child->element_type > 0
				&& child->element_type < UI_ACCEPT_AMOUNT
				&& g_acceptable[child->element_type].eventer)
				g_acceptable[child->element_type].eventer(child, e);
			else
				ft_printf("[%s] {%s} : Eventing of type %d %d is not supported.\n", __FUNCTION__, elem->id, curr->content_size, elem->element_type);
		}
		else
			ft_printf("[%s] Element [%s] @ [%d, %d] of type %d is not supported.\n", __FUNCTION__, child->screen_pos.x, child->screen_pos.y, child->id, curr->content_size);
		curr = curr->next;
	}
}

int	ui_menu_render(t_ui_element *elem)
{
	t_list			*curr;
	t_ui_menu		*menu;
	t_ui_element	*child;

	if (!*elem->parent_show || !elem->show)
		return (0);
	ui_element_render(elem);
	menu = elem->element;
	curr = menu->children;
	while (curr)
	{
		if (curr->content_size == UI_TYPE_ELEMENT)
		{
			child = curr->content;
			if (child->element_type > 0
				&& child->element_type < UI_ACCEPT_AMOUNT
				&& g_acceptable[child->element_type].renderer)
				g_acceptable[child->element_type].renderer(child);
			else
				ft_printf("[%s] {%s} : Rendering %s of type %d %d is not supported.\n", __FUNCTION__, elem->id, child->id, curr->content_size, elem->element_type);
		}
		else
			ft_printf("[ui_menu_render] Element isnt UI_TYPE_ELEMENT ... HOW? ... [%s] @ [%d, %d] of type %d is not supported.\n", child->screen_pos.x, child->screen_pos.y, child->id, curr->content_size);
		curr = curr->next;
	}
	return (1);
}

void	ui_menu_free(void *menu)
{
	(void)menu;
}

// getters

/*
 * returns element ,from children list, with id;
*/
t_ui_element	*ui_menu_get_element_by_id(t_ui_element *elem, char *id)
{
	return (ui_list_get_element_by_id(((t_ui_menu *)elem->element)->children, id));
}

/*
 * same as the ui_menu_get_element_by_id();
 * but this assumes the element is button since you get_button_...
 * so it will typecast it automatically for you;
*/
t_ui_button	*ui_menu_get_button_by_id(t_ui_element *elem, char *id)
{
	return (ui_list_get_element_by_id(((t_ui_menu *)elem->element)->children, id)->element);
}
