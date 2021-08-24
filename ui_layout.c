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

int	ui_layout_render(t_ui_layout *layout)
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
			int	j = -1;
			while (++j < UI_ACCEPT_AMOUNT)
			{
				if (g_acceptable[j].type == elem->element_type)
				{
					g_acceptable[j].renderer(elem);
					break ;
				}
			}
			if (j == UI_ACCEPT_AMOUNT)
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
	return (1);
}

t_ui_element	*ui_list_get_element_by_id(t_list *list, char *id)
{
	t_list			*curr;
	t_ui_element	*elem;

	curr = list;
	while (list)
	{
		elem = curr->content;
		if (ft_strequ(elem->id, id))
			return (elem);
		curr = curr->next;
	}
	return (NULL);
}

t_ui_element	*ui_layout_get_element_by_id(t_ui_layout *layout, char *id)
{
	return (ui_list_get_element_by_id(layout->elements, id));
}
