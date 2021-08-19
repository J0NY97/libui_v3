#include "libui_v3.h"

void	ui_layout_event(t_ui_layout *layout, SDL_Event e)
{
	t_list	*curr;
	t_ui_window	*win;

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
		if (curr->content_size == UI_TYPE_BUTTON)
			ui_button_event(curr->content, e);
		else if (curr->content_size == UI_TYPE_MENU)
			ui_menu_event(curr->content, e);
		else if (curr->content_size == UI_TYPE_LABEL)
			(void)curr;
		else
			ft_printf("[ui_layout_event] Eventing of type %d not supported.\n", curr->content_size);
		curr = curr->next;
	}

}

void	ui_layout_render(t_ui_layout *layout)
{
	t_list	*curr;
	t_ui_window	*win;

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
		if (curr->content_size == UI_TYPE_LABEL)
			ui_label_render(curr->content);
		else if (curr->content_size == UI_TYPE_BUTTON)
			ui_button_render(curr->content);
		else if (curr->content_size == UI_TYPE_MENU)
			ui_menu_render(curr->content);
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

void	ui_layout_get_element_by_id(t_ui_layout *layout, char *id)
{
	t_list	*curr;

	curr = layout->elements;
	while (curr)
	{
		if (ft_strequ(curr->content->id, id))
			return (
		curr = curr->next;
	}
}
