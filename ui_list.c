#include "libui.h"

/*
 *	sorts all elements with their z value in ascending order;
 *	TODO: figure out how often this should be done;
*/
void	ui_list_sort(t_list *list)
{
	t_list			*curr;
	t_list			*next;
	t_ui_element	*temp;
	int				temp_int;

	curr = list;
	while (curr)
	{
		next = curr->next;
		if (!next || !curr->content || !curr->next->content)
			break ;
		if (((t_ui_element *)curr->content)->z
			> ((t_ui_element *)curr->next->content)->z)
		{
			temp = curr->content;
			curr->content = curr->next->content;
			curr->next->content = temp;
			pointer_swap(&curr->content_size, &curr->next->content_size);
			curr = list;
		}
		else
			curr = curr->next;
	}
}

t_ui_recipe	*ui_list_get_recipe_by_id(t_list *list, char *id)
{
	t_list		*curr;
	t_ui_recipe	*recipe;

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

void	ui_list_event(t_list *list, SDL_Event e)
{
	t_list			*curr;
	t_ui_element	*elem;

	curr = list;
	while (curr)
	{
		elem = curr->content;
		if (elem->element_type >= 0 && elem->element_type < UI_TYPE_AMOUNT)
		{
			if (g_acceptable[elem->element_type].eventer)
				g_acceptable[elem->element_type].eventer(elem, e);
		}
		else
			ft_printf("[%s] Element of type %d (%s) not supported.\n",
				__FUNCTION__, elem->element_type,
				ui_element_type_to_string(elem->element_type));
		curr = curr->next;
	}
}

/*
 * A no matter what renderer.
*/
void	ui_list_render(t_list *list)
{
	t_list			*curr;
	t_ui_element	*elem;

	curr = list;
	while (curr)
	{
		elem = curr->content;
		if (elem->element_type >= 0
			&& elem->element_type < UI_TYPE_AMOUNT
			&& g_acceptable[elem->element_type].renderer)
			g_acceptable[elem->element_type].renderer(elem);
		else
			ft_printf("[%s] Element of type %d (%s) not supported.\n",
				__FUNCTION__, elem->element_type,
				ui_element_type_to_string(elem->element_type));
		curr = curr->next;
	}
}

/*
 * A matter what renderer.
*/
void	ui_layout_list_render(t_list *list)
{
	t_list			*curr;
	t_ui_element	*elem;

	curr = list;
	while (curr)
	{
		elem = curr->content;
		if (elem->element_type >= 0
			&& elem->element_type < UI_TYPE_AMOUNT
			&& !elem->is_a_part_of_another
			&& g_acceptable[elem->element_type].renderer)
			g_acceptable[elem->element_type].renderer(elem);
		else if (!elem->is_a_part_of_another)
			ft_printf("[%s] Element of type %d (%s) is not supported.\n",
				__FUNCTION__, elem->element_type,
				ui_element_type_to_string(elem->element_type));
		curr = curr->next;
	}
}

/*
 * Calls ui_element_free on everything in 'list'.
 * And then frees the whole list, no questions asked.
*/
void	ui_list_element_free(t_list	**list)
{
	t_list	*curr;
	t_list	*next;
	int		i;

	if (!list)
		return ;
	curr = *list;
	i = 0;
	while (curr)
	{
		next = curr->next;
		ui_element_free(curr->content, UI_TYPE_ELEMENT);
		if (!*list)
			break ;
		if (next)
			curr = next;
		else
			break ;
		i++;
	}
	ft_lstdel(list, &dummy_free_er);
}
