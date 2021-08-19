#include "libui_v3.h"

void	ui_menu_new(t_ui_window *win, t_ui_menu *menu)
{
	memset(menu, 0, sizeof(t_ui_menu));
	ui_element_new(win, &menu->elem);
	menu->children = NULL;
}

void	ui_menu_child_add(t_ui_menu *menu, void *child, int type)
{
	if (type == UI_TYPE_BUTTON)
		ui_element_parent_set(&((t_ui_button *)child)->elem, &menu->elem, UI_TYPE_ELEMENT, &menu->elem.show);
	else if (type == UI_TYPE_LABEL)
	{
		((t_ui_label *)child)->parent = &menu->elem;
		((t_ui_label *)child)->parent_type = UI_TYPE_ELEMENT;
		((t_ui_label *)child)->parent_show = &menu->elem.show;
	}
	else if (type == UI_TYPE_MENU)
		ui_element_parent_set(&((t_ui_menu *)child)->elem, &menu->elem, UI_TYPE_ELEMENT, &menu->elem.show);
	else if (type == UI_TYPE_DROPDOWN)
		ui_element_parent_set(&((t_ui_dropdown *)child)->elem, &menu->elem, UI_TYPE_ELEMENT, &menu->elem.show);
	add_to_list(&menu->children, child, type);	
}

void	ui_menu_event(t_ui_menu *menu, SDL_Event e)
{
	t_list	*curr;

	curr = menu->children;
	while (curr)
	{
		if (curr->content_size == UI_TYPE_BUTTON)
			ui_button_event(curr->content, e);
		else if (curr->content_size == UI_TYPE_MENU)
			ui_menu_event(curr->content, e);
		else if (curr->content_size == UI_TYPE_DROPDOWN)
			ui_dropdown_event(curr->content, e);
		curr = curr->next;
	}
}

void	ui_menu_render(t_ui_menu *menu)
{
	t_list	*curr;

	if (!*menu->elem.parent_show || !menu->elem.show)
		return ;
	ui_element_render(&menu->elem);
	curr = menu->children;
	while (curr)
	{
		if (curr->content_size == UI_TYPE_BUTTON)
			ui_button_render(curr->content);
		else if (curr->content_size == UI_TYPE_LABEL)
			ui_label_render(curr->content);
		else if (curr->content_size == UI_TYPE_MENU)
			ui_menu_render(curr->content);
		else if (curr->content_size == UI_TYPE_DROPDOWN)
			ui_dropdown_render(curr->content);
		curr = curr->next;
	}
}

void	ui_menu_free(void *menu)
{
	(void)menu;
}
