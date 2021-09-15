#include "libui.h"

void	ui_layout_event(t_ui_layout *layout, SDL_Event e)
{
	t_list			*curr;
	t_ui_window		*win;
	t_ui_element	*elem;

	curr = layout->windows;
	while (curr)
	{
		win = curr->content;
		if (!win->user_handled_event)
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
			int	j = -1;
			while (++j < UI_TYPE_AMOUNT)
			{
				if (g_acceptable[j].type == elem->element_type)
				{
					if (g_acceptable[j].eventer)
						g_acceptable[j].eventer(elem, e);
					break ;
				}
			}
			if (j == UI_TYPE_AMOUNT)
				ft_printf("[%s] Eventing of type %d %d is not supported.\n", __FUNCTION__, curr->content_size, elem->element_type);
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
			while (++j < UI_TYPE_AMOUNT)
			{
				if (g_acceptable[j].type == elem->element_type)
				{
					if (g_acceptable[j].renderer)
						g_acceptable[j].renderer(elem);
					break ;
				}
			}
			if (j == UI_TYPE_AMOUNT)
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
		ui_window_render(win);
		SDL_RenderPresent(win->renderer);
		curr = curr->next;
	}
	return (1);
}

t_ui_recipe_v2	*ui_list_get_recipe_by_id_v2(t_list *list, char *id)
{
	t_list			*curr;
	t_ui_recipe_v2	*recipe;

	curr = list;
	while (curr)
	{
		recipe = curr->content;
		if (ft_strequ(recipe->id, id))
			return (recipe);
		curr = curr->next;
	}
	ft_printf("[%s] Couldn\'t find recipe with id : %s\n", __FUNCTION__, id);
	return (NULL);
}

t_ui_element	*ui_list_get_element_by_id(t_list *list, char *id)
{
	t_list			*curr;
	t_ui_element	*elem;

	curr = list;
	while (curr)
	{
		elem = curr->content;
		if (ft_strequ(elem->id, id))
			return (elem);
		curr = curr->next;
	}
	ft_printf("[%s] Couldn\'t find element with id : %s\n", __FUNCTION__, id);
	return (NULL);
}

t_ui_element	*ui_layout_get_element_by_id(t_ui_layout *layout, char *id)
{
	return (ui_list_get_element_by_id(layout->elements, id));
}

t_ui_window	*ui_list_get_window_by_id(t_list *list, char *id)
{
	t_ui_window	*win;

	while (list)
	{
		win = list->content;
		if (ft_strequ(win->id, id))
			return (win);
		list = list->next;
	}
	ft_printf("[%s] Couldn\'t find window with id : %s\n", __FUNCTION__, id);
	return (NULL);
}

t_ui_window	*ui_layout_get_window_by_id(t_ui_layout *layout, char *id)
{
	return (ui_list_get_window_by_id(layout->windows, id));
}
