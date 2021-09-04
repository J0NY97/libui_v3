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
			int	j = -1;
			while (++j < UI_ACCEPT_AMOUNT)
			{
				if (g_acceptable[j].type == child->element_type
					&& g_acceptable[j].eventer)
				{
					g_acceptable[j].eventer(child, e);
					break ;
				}
			}
			if (j == UI_ACCEPT_AMOUNT)
				ft_printf("[%s] Eventing of type %d %d is not supported.\n", __FUNCTION__, curr->content_size, elem->element_type);
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
			int	j = -1;
			while (++j < UI_ACCEPT_AMOUNT)
			{
				if (g_acceptable[j].type == child->element_type
					&& g_acceptable[j].renderer)
				{
					g_acceptable[j].renderer(child);
					break ;
				}
			}
			if (j == UI_ACCEPT_AMOUNT)
				ft_printf("[%s] Rendering of type %d %d is not supported.\n", __FUNCTION__, curr->content_size, elem->element_type);
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
