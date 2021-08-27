#include "libui_v3.h"

/*
 * NOTE: the reason dropdown has element, is becuase of the parent stuff,
 * 	we dont actually need to show anything thats why the size of it is 0 x 0
*/
void	ui_dropdown_new(t_ui_window *win, t_ui_element *elem)
{
	t_ui_dropdown	*drop;

	ui_element_new(win, elem);
	elem->element = ft_memalloc(sizeof(t_ui_dropdown));
	elem->element_type = UI_TYPE_DROPDOWN;
	drop = elem->element;

	ui_element_color_set(elem, UI_STATE_DEFAULT, 0xffff0000);
	ui_element_pos_set(elem, vec4(0, 0, 50, 20));

	ui_label_new(win, &drop->label);
	ui_label_text_set(&drop->label, "Dropdown");

	ui_menu_new(win, &drop->menu);
	ui_element_pos_set(&drop->menu, vec4(0, elem->pos.h, drop->menu.pos.w, drop->menu.pos.h));
	drop->menu.show = 0;

	ui_element_parent_set(&drop->label, elem, UI_TYPE_ELEMENT, &elem->show);
	ui_element_parent_set(&drop->menu, elem, UI_TYPE_ELEMENT, &elem->show);
}

void	ui_dropdown_event(t_ui_element *elem, SDL_Event e)
{
	t_ui_dropdown	*drop;

	drop = elem->element;
	ui_button_event(elem, e);
	if (ui_button(elem))
		drop->menu.show = drop->menu.show == 0;
	if (drop->menu.show)
		elem->state = UI_STATE_CLICK;
	ui_menu_event(&drop->menu, e);
}

int	ui_dropdown_render(t_ui_element *elem)
{
	t_ui_dropdown	*drop;

	drop = elem->element;
	if (!ui_button_render(elem))
		return (0);

	ui_element_pos_set(&drop->menu, vec4(0, elem->pos.h, drop->menu.pos.w, drop->menu.pos.h));
	ui_menu_render(&drop->menu);
	return (1);
}

void	ui_dropdown_free(void *drop)
{
	(void)drop;
}
