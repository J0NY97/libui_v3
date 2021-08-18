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
		if (curr->content_size == sizeof(t_ui_button))
			ui_button_event(curr->content, e);
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
		if (curr->content_size == sizeof(t_ui_label))
			ui_label_render(curr->content);
		else if (curr->content_size == sizeof(t_ui_button))
			ui_button_render(curr->content);
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
