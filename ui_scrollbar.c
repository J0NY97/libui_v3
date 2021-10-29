#include "libui.h"

void	ui_scrollbar_new(t_ui_window *win, t_ui_element *elem)
{
	t_ui_scrollbar	*scroll;

	ui_element_new(win, elem);
	elem->element = ft_memalloc(sizeof(t_ui_scrollbar));
	elem->element_type = UI_TYPE_SCROLLBAR;

	scroll = elem->element;

	ui_button_new(win, &scroll->button);
	ui_element_set_parent(&scroll->button, elem, UI_TYPE_ELEMENT);
	ui_element_pos_set(&scroll->button, vec4(0, 0, elem->pos.h, elem->pos.h));
	ui_element_color_set(&scroll->button, UI_STATE_DEFAULT, UI_COLOR_ORANGEISH);
	ui_element_color_set(&scroll->button, UI_STATE_HOVER, UI_COLOR_ORANGEISH_DARKER);
	ui_element_color_set(&scroll->button, UI_STATE_CLICK, UI_COLOR_ORANGEISH_DARKEST);

	ui_label_size_set(&ui_scrollbar_get_button(elem)->label, 20);
	ui_label_text_set(&ui_scrollbar_get_button(elem)->label, "=");

	scroll->min = 0;
	scroll->max = 100;
	ui_scroll_value_set(elem, 50);
	scroll->update = 1;
}

void	ui_scrollbar_event(t_ui_element *elem, SDL_Event e)
{
	t_ui_scrollbar	*scroll;
	t_ui_button		*button;

	if (!elem->show)
		return ;
	scroll = elem->element;
	button = scroll->button.element;
	if (ui_element_is_click(elem))
	{
		scroll->value = ui_get_slider_value(scroll->min, scroll->max,
				(elem->win->mouse_pos.y - elem->screen_pos.y) - scroll->button.pos.h / 2,
				elem->pos.h - scroll->button.pos.h);
		scroll->update = 1;
	}
}

int	ui_scrollbar_render(t_ui_element *elem)
{
	t_ui_scrollbar	*scroll;
	t_ui_button		*button;

	scroll = elem->element;
	button = scroll->button.element;

	if (!ui_element_render(elem))
		return (0);
	ui_scrollbar_recount(elem);
	if (scroll->update)
	{
		scroll->button.pos.y = ft_clamp(ui_set_slider_value(scroll->value, scroll->min, scroll->max, elem->pos.h - scroll->button.pos.h), 0, elem->pos.h - scroll->button.pos.h);
		scroll->update = 0;
		ui_element_move_list(((t_ui_element *)scroll->target)->children, vec2i(0, scroll->last_value - scroll->value));
	}
	scroll->last_value = scroll->value;
	ui_button_render(&scroll->button);
	return (1);
}

void	ui_scrollbar_edit(t_ui_element *elem, t_ui_recipe *recipe)
{
	t_ui_scrollbar	*scroll;

	scroll = elem->element;
	if (recipe->target)
		scroll->target = ui_list_get_element_by_id(elem->win->layout->elements, recipe->target);
	if (recipe->value_set[1])
		scroll->min = recipe->value[1];
	if (recipe->value_set[2])
		scroll->max = recipe->value[2];
	if (recipe->value_set[0])
		ui_scroll_value_set(elem, recipe->value[0]);
}

void	ui_scrollbar_free(void *args)
{
	(void)args;
}

void	ui_scroll_value_set(t_ui_element *elem, int value)
{
	t_ui_scrollbar	*scroll;

	scroll = elem->element;
	scroll->value = value;
	scroll->button.pos.y = ft_clamp(ui_set_slider_value(value, scroll->min, scroll->max, elem->pos.h - scroll->button.pos.h), 0, elem->pos.h - scroll->button.pos.h);
	scroll->update = 1;
}

// Help
/*
 * recalculates the value of the list of childrens size;
*/
void	ui_scrollbar_recount(t_ui_element *elem)
{
	t_list			*curr;
	t_ui_scrollbar	*scroll;
	t_ui_element	*child;
	int				min;
	int				max;

	scroll = elem->element;
	curr = ((t_ui_element *)scroll->target)->children;
	min = INT_MAX;
	max = INT_MIN;
	while (curr)
	{
		child = curr->content;
		if (child->pos.y < min)
			min = child->pos.y;
		if (child->pos.y + child->pos.h > max)
			max = child->pos.y + child->pos.h;
		curr = curr->next;
	}
	scroll->target_size = abs(min - max);
	if (((t_ui_element *)scroll->target)->pos.h - scroll->target_size < 0)
		scroll->max = abs(((t_ui_element *)scroll->target)->pos.h - scroll->target_size);
	else
		scroll->max = 0;
	int new_h = (((t_ui_element *)scroll->target)->pos.h / scroll->target_size) * elem->pos.h;
	new_h = ft_clamp(elem->pos.h, elem->pos.h * 0.05, new_h);
	ui_element_pos_set(&scroll->button,
		vec4(scroll->button.pos.x, scroll->button.pos.y, scroll->button.pos.w, new_h));
	scroll->top_most = vec2i(0, 0);
	scroll->bot_most = vec2i(0, 0);
	if (((t_ui_element *)scroll->target)->children)
	{
		scroll->top_most = vec2i(0, min);
		scroll->bot_most = vec2i(0, max);
	}
}

t_ui_element	*ui_scrollbar_get_button_element(t_ui_element *elem)
{
	if (elem->element_type != UI_TYPE_SCROLLBAR)
	{
		ft_printf("[%s] elem type of %d %s given instead of scrollbar.\n", __FUNCTION__, elem->element_type, ui_element_type_to_string(elem->element_type));
		return (NULL);
	}
	return (&((t_ui_scrollbar *)elem->element)->button);
}

t_ui_button	*ui_scrollbar_get_button(t_ui_element *elem)
{
	if (elem->element_type != UI_TYPE_SCROLLBAR)
	{
		ft_printf("[%s] elem type of %d %s given instead of scrollbar.\n", __FUNCTION__, elem->element_type, ui_element_type_to_string(elem->element_type));
		return (NULL);
	}
	return (((t_ui_scrollbar *)elem->element)->button.element);
}
