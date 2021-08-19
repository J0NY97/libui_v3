#include "libui_v3.h"

void	ui_menu_new(t_ui_window *win, t_ui_element *elem)
{
	ui_element_new(win, elem);
	elem->element = ft_memalloc(sizeof(t_ui_menu));
	elem->element_type = UI_TYPE_MENU;
	((t_ui_menu *)elem->element)->children = NULL;
	((t_ui_menu *)elem->element)->elem = elem;
}

void	ui_menu_child_add(t_ui_element *elem, void *child, int type)
{
	if (type == UI_TYPE_ELEMENT)
		ui_element_parent_set(child, elem, UI_TYPE_ELEMENT, &elem->show);
	else
	{
		ft_printf("[ui_menu_child_add] child of type %d is not supported.\n", type);
		return ;
	}
	add_to_list(&((t_ui_menu *)elem->element)->children, child, type);
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
			if (child->element_type == UI_TYPE_BUTTON)
				ui_button_event(child, e);
			else if (child->element_type == UI_TYPE_MENU)
				ui_menu_event(child, e);
			else if (child->element_type == UI_TYPE_DROPDOWN)
				ui_dropdown_event(child, e);
			else if (child->element_type == UI_TYPE_LABEL)
				(void)curr;
			else
				ft_printf("[ui_menu_event] Element [%s] @ [%d, %d] of type %d %d is not supported.\n", child->screen_pos.x, child->screen_pos.y, child->id, curr->content_size, child->element_type);
		}
		else
			ft_printf("[ui_menu_event] Element [%s] @ [%d, %d] of type %d is not supported.\n", child->screen_pos.x, child->screen_pos.y, child->id, curr->content_size);
		curr = curr->next;
	}
}

void	ui_menu_render(t_ui_element *elem)
{
	t_list			*curr;
	t_ui_menu		*menu;
	t_ui_element	*child;

	if (!*elem->parent_show || !elem->show)
		return ;
	ui_element_render(elem);
	menu = elem->element;
	curr = menu->children;
	while (curr)
	{
		if (curr->content_size == UI_TYPE_ELEMENT)
		{
			child = curr->content;
			if (child->element_type == UI_TYPE_BUTTON)
				ui_button_render(child);
			else if (child->element_type == UI_TYPE_LABEL)
				ui_label_render(curr->content);
			else if (child->element_type == UI_TYPE_MENU)
				ui_menu_render(child);
			else if (child->element_type == UI_TYPE_DROPDOWN)
				ui_dropdown_render(child);
			else
				ft_printf("[ui_menu_render] Element [%s] @ [%d, %d] of type %d %d is not supported.\n", child->screen_pos.x, child->screen_pos.y, child->id, curr->content_size, child->element_type);
		}
		else
			ft_printf("[ui_menu_render] Element isnt UI_TYPE_ELEMENT ... HOW? ... [%s] @ [%d, %d] of type %d is not supported.\n", child->screen_pos.x, child->screen_pos.y, child->id, curr->content_size);
		curr = curr->next;
	}
}

void	ui_menu_free(void *menu)
{
	(void)menu;
}
