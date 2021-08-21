#include "libui_v3.h"

void	ui_layout_event(t_ui_layout *layout, SDL_Event e)
{
	t_list			*curr;
	t_ui_window		*win;
	t_ui_element	*elem;

	curr = layout->windows;
	while (curr)
	{
		win = curr->content;
		ui_window_event(win, e);
		curr = curr->next;
	}
	//////////////////
	// Event elements
	//////////////////
	curr = layout->elements;
	while (curr)
	{
		if (curr->content_size == UI_TYPE_ELEMENT)
		{
			elem = curr->content;
			if (elem->element_type == UI_TYPE_BUTTON)
				ui_button_event(elem, e);
			else if (elem->element_type == UI_TYPE_MENU)
				ui_menu_event(elem, e);
			else if (elem->element_type == UI_TYPE_DROPDOWN)
				ui_dropdown_event(elem, e);
			else if (elem->element_type == UI_TYPE_LABEL)
				(void)curr;
			else
				ft_printf("[ui_layout_event] Eventing of type %d %d not supported.\n", curr->content_size, elem->element_type);
		}
		else
			ft_printf("[ui_layout_event] Eventing of type %d not supported.\n", curr->content_size);
		curr = curr->next;
	}
}

void	ui_layout_render(t_ui_layout *layout)
{
	t_list			*curr;
	t_ui_window		*win;
	t_ui_element	*elem;

	curr = layout->windows;
	while (curr)
	{
		win = curr->content;
		SDL_RenderClear(win->renderer);
		ui_window_render(win);
		curr = curr->next;
	}
	/////////////////////////////
	// Render all the elements
	/////////////////////////////
	curr = layout->elements;
	while (curr)
	{
		if (curr->content_size == UI_TYPE_ELEMENT)
		{
			elem = curr->content;
			ft_printf("Rendering Element ID : %s\n", elem->id);
			if (elem->element_type == UI_TYPE_BUTTON)
				ui_button_render(elem);
			else if (elem->element_type == UI_TYPE_MENU)
				ui_menu_render(elem);
			else if (elem->element_type == UI_TYPE_LABEL)
				ui_label_render(curr->content);
			else
				ft_printf("[ui_layout_render] Rendering of type %d %d is not supported.\n", curr->content_size, elem->element_type);
		}
		else
			ft_printf("[ui_layout_render] Rendering of type %d is not supported.\n", curr->content_size);
		curr = curr->next;
	}
	/////////////////////////////
	// Stop rendering
	/////////////////////////////
	curr = layout->windows;
	while (curr)
	{
		win = curr->content;
		SDL_RenderPresent(win->renderer);
		curr = curr->next;
	}
}

t_ui_element	*ui_layout_get_element_by_id(t_ui_layout *layout, char *id)
{
	t_list			*curr;
	t_ui_element	*elem;

	curr = layout->elements;
	ft_printf("lets see if we can find element with id : %s\n", id);
	while (curr)
	{
		elem = curr->content;
		ft_printf("checking id : %s of type %d @ pos ", elem->id, elem->element_type);
		print_veci(elem->pos.v, 4);
		/*
		if (elem->id && ft_strequ(elem->id, id))
			return (curr->content);
			*/
		curr = curr->next;
	}
	return (NULL);
}
