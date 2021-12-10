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
	ui_list_event(layout->elements, e);
}

void	ui_layout_render(t_ui_layout *layout)
{
	t_list			*curr;
	t_ui_window		*win;

	ui_list_sort(layout->elements);
	ui_layout_list_render(layout->elements);
	curr = layout->windows;
	while (curr)
	{
		win = curr->content;
		ui_window_render(win);
		curr = curr->next;
	}
}

void	ui_layout_load(t_ui_layout *layout, char *file)
{
	memset(layout, 0, sizeof(t_ui_layout));
	layout->layout_file = ft_strdup(file);
	layout->layout_file_content = get_file_content(layout, file);
	if (layout->layout_file_content == NULL)
		return ;
	layout->layout_element_strings
		= split_string_into_array(layout->layout_file_content);
	layout_make_family_trees(layout);
	layout_compile_elements(layout);
	if (!layout->style_file)
		return ;
	layout->style_file_content
		= get_file_content(layout, layout->style_file);
	if (layout->style_file_content == NULL)
		return ;
	ft_printf("[%s] Style file content got\n", __FUNCTION__);
	layout->style_recipe_strings
		= split_string_into_array(layout->style_file_content);
	ft_printf("[%s] Recipe strings made.\n", __FUNCTION__);
	layout_make_recipes(layout);
	ft_printf("[%s] Recipe made.\n", __FUNCTION__);
	layout_apply_style(layout);
	ft_printf("[%s] Recipes applied.\n", __FUNCTION__);
	ft_printf("[%s] Resource dirs.\n", __FUNCTION__);
	ft_putarr(layout->resource_dirs);
}

/*
 * Layout shouldnt be malloced, so dont free it;
 * 	(if its malloced, the user has done it, so he has to do it;
*/
void	ui_layout_free(t_ui_layout *layout)
{
	if (!layout)
		return ;
	free(layout->layout_file);
	free(layout->layout_file_content);
	ft_arraydel(layout->layout_element_strings);
	free(layout->style_file);
	free(layout->style_file_content);
	ft_arraydel(layout->style_recipe_strings);
	ft_lstdel(&layout->windows, &ui_window_free);
	ft_lstdel(&layout->elements, &dummy_free_er);
	ft_lstdel(&layout->families, &ui_family_free);
	ft_lstdel(&layout->recipes, &ui_recipe_free);
	ft_arraydel(layout->resource_dirs);
}

/*
 * Applies all the recipes to the corresponding elements.
*/
void	layout_apply_style(t_ui_layout *layout)
{
	t_list			*curr;
	t_ui_window		*win;
	t_ui_element	*elem;
	t_ui_recipe		*recipe;

	curr = layout->windows;
	while (curr)
	{
		win = curr->content;
		recipe = ui_list_get_recipe_by_id(layout->recipes, win->id);
		if (win && recipe)
			ui_window_edit(win, recipe);
		curr = curr->next;
	}
	curr = layout->elements;
	while (curr)
	{
		elem = curr->content;
		recipe = ui_list_get_recipe_by_id(layout->recipes, elem->id);
		if (elem && recipe)
			ui_element_edit(elem, recipe);
		curr = curr->next;
	}
}
