#include "libui_v3.h"

void	ui_menu_new(t_ui_window *win, t_ui_menu *menu)
{
	ui_element_new(win, &menu->elem);
	menu->children = NULL;
}

void	ui_menu_child_add(t_ui_menu *menu, void *child)
{
	((t_ui_element *)child)->parent = &menu->elem;
	((t_ui_element *)child)->parent_type = UI_TYPE_ELEMENT;
	add_to_list(&menu->children, child, sizeof(t_ui_element));	
}

void	ui_menu_render(t_ui_menu *menu)
{
	t_list	*curr;

	ui_element_render(&menu->elem);
	curr = menu->children;	
	while (curr)
	{
		ui_element_render(curr->content);
		curr = curr->next;
	}
}

void	ui_menu_get(void *menu)
{
	(void)menu;
}

void	ui_menu_free(void *menu)
{
	(void)menu;
}
