#include "libui.h"

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
