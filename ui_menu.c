#include "libui_v3.h"

void	ui_menu_new(t_ui_window *win, t_ui_menu *menu)
{
	ui_element_new(win, &menu->elem);
}

void	ui_menu_add_child(t_ui_menu *menu, void *child)
{
	add_to_list(menu->children, child, sizeof(t_ui_element));	
}

void	ui_menu_render(t_ui_menu *menu)
{
	(void)menu;
}

void	ui_menu_get(void *menu)
{
	(void)menu;
}

void	ui_menu_free(void *menu)
{
	(void)menu;
}
