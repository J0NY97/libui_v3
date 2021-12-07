#include "libui.h"

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
	drop->label.is_a_part_of_another = 1;
	ui_label_set_text(&drop->label, "Dropdown");
	ui_element_set_parent(&drop->label, elem, UI_TYPE_ELEMENT);

	ui_menu_new(win, &drop->menu);
	drop->menu.is_a_part_of_another = 1;
	ui_element_pos_set(&drop->menu,
		vec4(0, elem->pos.h, drop->menu.pos.w, drop->menu.pos.h));
	drop->menu.show = 0;
	((t_ui_menu *)drop->menu.element)->event_children = 1;
	((t_ui_menu *)drop->menu.element)->render_children = 1;
	ui_element_set_parent(&drop->menu, elem, UI_TYPE_ELEMENT);

	drop->max_h = 100;
	ui_scrollbar_new(win, &drop->scrollbar);
	drop->scrollbar.is_a_part_of_another = 1;
	ui_element_set_parent(&drop->scrollbar, elem, UI_TYPE_ELEMENT);
	ui_scroll_value_set(&drop->scrollbar, 0);
	((t_ui_scrollbar *)drop->scrollbar.element)->target = &drop->menu;
	((t_ui_scrollbar *)drop->scrollbar.element)->top_most = vec2i(0, 0);
}

void	ui_dropdown_edit(t_ui_element *elem, t_ui_recipe *recipe)
{
	t_ui_dropdown	*drop;

	if (!elem || !recipe)
		return ;
	drop = elem->element;
	ui_label_edit(&drop->label, recipe);
}

void	ui_dropdown_event(t_ui_element *elem, SDL_Event e)
{
	t_ui_dropdown	*drop;

	drop = elem->element;
	ui_checkbox_event(elem, e);
	// this is getting kind of convoluted, so thats why we have these both;
	// (im not sure which of these we should have);
	drop->menu.show = elem->state == UI_STATE_CLICK;
	drop->menu.show = elem->is_toggle;

	drop->menu.z = elem->z;

	if (!drop->menu.show)// || !drop->menu.children) // if menu isnt shown, no point event handling children; // or not;
	{
		ui_checkbox_toggle_off(elem);
		drop->scrollbar.show = 0;
		return ;
	}
	drop->drop_open = elem->win->mouse_down_last_frame && ui_element_is_hover(elem);
	ui_menu_event(&drop->menu, e);
	ui_list_radio_event(drop->menu.children, &drop->active);
	if (drop->active)
	{
		ui_element_render(drop->active);
		ui_label_set_text(&drop->label, ui_dropdown_active_text(elem));
	}
	{
		// This could be changed to the menu in the same wave as you render or event handle the children.
		t_list *curr;
		t_ui_element *child;
		curr = drop->menu.children;
		int total_height = 0;
		while (curr)
		{
			child = curr->content;
			ui_element_pos_set2(child, vec2(child->pos.x, total_height
					- ((t_ui_scrollbar *)drop->scrollbar.element)->value));
			total_height += child->pos.h;
			child->render_me_on_parent = 1; // making all the children render on parent;
			curr = curr->next;
		}
		if (drop->max_h != -1 && total_height > drop->max_h)
		{
			drop->scrollbar.show = 1;
			ui_scrollbar_event(&drop->scrollbar, e);
		}
		else
			drop->scrollbar.show = 0;
		ui_element_pos_set(&drop->menu,
			vec4(drop->menu.pos.x, drop->menu.pos.y,
				drop->menu.pos.w, total_height + 1));
	}
	if (elem->win->mouse_down // close the menu if you click somewhere else but the menu of dropdown;
		&& !ui_element_is_hover(elem)
		&& !ui_element_is_hover(&drop->scrollbar))
	{
		ui_checkbox_toggle_off(elem);
		drop->menu.show = 0;
		drop->drop_exit = 1;
		drop->scrollbar.show = 0;
	}
}

int	ui_dropdown_render(t_ui_element *elem)
{
	t_ui_dropdown	*drop;
	t_ui_element	*scroll_button;

	if (!ui_button_render(elem))
		return (0);
	drop = elem->element;
	drop->drop_exit = 0;
	ui_element_pos_set(&drop->menu,
		vec4(0, elem->pos.h,
			drop->menu.pos.w, ft_min(drop->max_h, drop->menu.pos.h)));
	ui_menu_render(&drop->menu);
	ui_element_pos_set(&drop->scrollbar,
		vec4(drop->menu.pos.x + drop->menu.pos.w, elem->pos.h,
			10, drop->menu.pos.h));
	scroll_button = ui_scrollbar_get_button_element(&drop->scrollbar);
	ui_element_pos_set(scroll_button,
		vec4(scroll_button->pos.x, scroll_button->pos.y,
			drop->scrollbar.pos.w, scroll_button->pos.h));
	ui_scrollbar_render(&drop->scrollbar);
	return (1);
}

void	ui_dropdown_free(void *elem, size_t size)
{
	t_ui_element	*element;
	t_ui_dropdown	*dropdown;

	element = elem;
	if (!element)
		return ;
	dropdown = element->element;
	if (!dropdown)
		return ;
	ui_element_free(&dropdown->label, UI_TYPE_LABEL);
	ui_element_free(&dropdown->menu, UI_TYPE_MENU);
	ui_element_free(&dropdown->scrollbar, UI_TYPE_SCROLLBAR);
	free(dropdown);
	(void)size;
}

void	ui_dropdown_activate(t_ui_element *drop, t_ui_element *elem)
{
	t_list			*curr;
	t_ui_dropdown	*d;

	if (!elem)
		return ;
	d = ui_dropdown_get_dropdown(drop);
	if (!drop || !d)
		return ;
	curr = ui_dropdown_get_menu_element(drop)->children;
	while (curr)
	{
		if (elem == curr->content)
		{
			d->active = elem;
			ui_element_render(d->active);
			ui_label_set_text(&d->label, ui_button_get_label(d->active)->text);
			break ;
		}
		curr = curr->next;
	}
}

/*
 * This will return 0/1 every frame.
*/
int	ui_dropdown_is_open(t_ui_element *elem)
{
	if (elem->element_type != UI_TYPE_DROPDOWN)
	{
		ft_printf("[%s] Elem is not of type UI_TYPE_DROPDOWN.[%s, %s]\n",
			__FUNCTION__, elem->id,
			ui_element_type_to_string(elem->element_type));
		return (-1);
	}
	return (((t_ui_dropdown *)elem->element)->menu.show);
}

/*
 * This will only return 1 the frame the menu was opened.
*/
int	ui_dropdown_open(t_ui_element *elem)
{
	if (elem->element_type != UI_TYPE_DROPDOWN)
	{
		ft_printf("[%s] Elem is not of type UI_TYPE_DROPDOWN.[%s, %s]\n",
			__FUNCTION__, elem->id,
			ui_element_type_to_string(elem->element_type));
		return (-1);
	}
	return (((t_ui_dropdown *)elem->element)->drop_open);
}

int	ui_dropdown_exit(t_ui_element *elem)
{
	if (elem->element_type != UI_TYPE_DROPDOWN)
	{
		ft_printf("[%s] Elem is not of type UI_TYPE_DROPDOWN.[%s, %s]\n",
			__FUNCTION__, elem->id,
			ui_element_type_to_string(elem->element_type));
		return (-1);
	}
	return (((t_ui_dropdown *)elem->element)->drop_exit);
}

t_ui_element	*ui_dropdown_get(t_ui_element *elem, int ui_type)
{
	if (elem->element_type == UI_TYPE_DROPDOWN)
	{
		if (ui_type == UI_TYPE_MENU)
			return (ui_dropdown_get_menu_element(elem));
		if (ui_type == UI_TYPE_SCROLLBAR)
			return (ui_dropdown_get_scrollbar_element(elem));
	}
	ft_printf("[%s] Something went wrong.\n", __FUNCTION__);
	return (NULL);
}

t_ui_menu	*ui_dropdown_get_menu(t_ui_element *elem)
{
	t_ui_dropdown	*drop;
	t_ui_menu		*menu;

	drop = ui_dropdown_get_dropdown(elem);
	if (!drop)
		return (NULL);
	return (ui_menu_get_menu(&drop->menu));
}

// Getters

t_ui_element	*ui_dropdown_get_menu_element(t_ui_element *elem)
{
	if (elem->element_type == UI_TYPE_DROPDOWN)
		return (&ui_dropdown_get_dropdown(elem)->menu);
	ft_printf("[%s] Something went wrong.\n", __FUNCTION__);
	return (NULL);
}

t_ui_element	*ui_dropdown_get_scrollbar_element(t_ui_element *elem)
{
	if (elem->element_type == UI_TYPE_DROPDOWN)
		return (&ui_dropdown_get_dropdown(elem)->scrollbar);
	ft_printf("[%s] Something went wrong.\n", __FUNCTION__);
	return (NULL);
}

/*
 * Returns dropdown from element, if the type is dropdown;
*/
t_ui_dropdown	*ui_dropdown_get_dropdown(t_ui_element *elem)
{
	if (!elem || elem->element_type != UI_TYPE_DROPDOWN)
		return (NULL);
	return (elem->element);
}

t_ui_element	*ui_dropdown_active(t_ui_element *elem)
{
	t_ui_dropdown	*drop;

	if (!elem)
		return (NULL);
	if (elem->element_type != UI_TYPE_DROPDOWN)
	{
		ft_printf("[%s] Elem not of type UI_TYPE_DROPDOWN.%d %s.\n",
			__FUNCTION__, elem->element_type,
			ui_element_type_to_string(elem->element_type));
		return (NULL);
	}
	return (ui_dropdown_get_dropdown(elem)->active);
}

char	*ui_dropdown_active_text(t_ui_element *elem)
{
	t_ui_element	*active;
	t_ui_label		*label;

	if (!elem)
		return (NULL);
	active = ui_dropdown_get_dropdown(elem)->active;
	if (!active)
		return (NULL);
	label = ui_button_get_label(active);
	if (!label)
		return (NULL);
	return (label->text);
}

t_ui_element	*ui_dropdown_get_button_with_text(t_ui_element *elem, char *text)
{
	t_ui_element	*menu;

	if (!elem || elem->element_type != UI_TYPE_DROPDOWN || !text)
		return (NULL);
	menu = ui_dropdown_get_menu_element(elem);
	if (!menu)
		return (NULL);
	return (ui_list_get_button_with_text(menu->children, text));
}
