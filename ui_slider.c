#include "libui.h"

void	ui_slider_new(t_ui_window *win, t_ui_element *elem)
{
	t_ui_slider	*slider;
	t_ui_button	*button;

	ui_element_new(win, elem);
	elem->element = ft_memalloc(sizeof(t_ui_slider));
	elem->element_type = UI_TYPE_SLIDER;
	slider = elem->element;
	slider->elem = elem;

	ui_button_new(win, &slider->button);
	ui_element_pos_set(&slider->button, vec4(0, 0, elem->pos.h, elem->pos.h));
	ui_element_color_set(&slider->button, UI_STATE_DEFAULT, UI_COLOR_ORANGEISH);
	ui_element_color_set(&slider->button, UI_STATE_HOVER, UI_COLOR_ORANGEISH_DARKER);
	ui_element_color_set(&slider->button, UI_STATE_CLICK, UI_COLOR_ORANGEISH_DARKEST);
	ui_element_parent_set(&slider->button, elem, UI_TYPE_ELEMENT);
	button = slider->button.element;

	slider->value = 50;
	slider->min_value = 0;
	slider->max_value = 100;

	slider->update = 1;
}

void	ui_slider_edit(t_ui_element *elem, t_ui_recipe_v2 *recipe)
{
	t_ui_slider	*slider;
	t_ui_window	*win;
	t_ui_recipe_v2	*button_recipe;

	win = elem->win;
	slider = elem->element;
	if (recipe->button_id)
	{
		ft_printf("[%s] button id is.\n", __FUNCTION__);
		button_recipe = ui_list_get_recipe_by_id_v2(win->layout->recipes, recipe->button_id);
		if (button_recipe)
			ui_element_edit(&slider->button, button_recipe);
		else
			ft_printf("[%s] Slider button [%s] couldn\'t be found orseomthign.\n", __FUNCTION__, recipe->button_id);
	}
}

int	ui_slider_value_get(t_ui_element *elem)
{
	t_ui_slider	*slider;

	slider = elem->element;
	return (ui_get_slider_value(slider->min_value, slider->max_value, slider->button.pos.x, elem->pos.w - slider->button.pos.w));
}

void	ui_slider_value_set(t_ui_element *elem, int value)
{
	t_ui_slider	*slider;

	slider = elem->element;
	slider->value = value;
	slider->button.pos.x = ft_clamp(ui_set_slider_value(value, slider->min_value, slider->max_value, elem->pos.w - slider->button.pos.w), 0, elem->pos.w - slider->button.pos.w);
	slider->update = 1;
}

int	ui_get_slider_value(int min, int max, int pos_x, int w)
{
	float	pixels_per_value;
	int		total_values;
	int		nth_value;
	int		curr_value;

	total_values = abs(max - min);
	pixels_per_value = (float)total_values / (float)w;
	nth_value = pixels_per_value * (float)pos_x;
	curr_value = min + nth_value;
	return (ft_clamp(curr_value, min, max));
}

int	ui_set_slider_value(int value, int min, int max, int w)
{
	return (ft_clamp(((float)w / (float)(abs(max - min)))
		* (float)(abs(min - value)), 0, w));
}

void	ui_slider_event(t_ui_element *elem, SDL_Event e)
{
	t_ui_slider	*slider;
	t_ui_button	*button;

	if (!elem->show)
		return ;
	slider = elem->element;
	button = slider->button.element;
	if (e.type == SDL_MOUSEBUTTONDOWN
		&& point_in_rect(elem->win->mouse_pos, elem->screen_pos))
		ui_element_pos_set2(&slider->button, vec2(elem->win->mouse_pos.x - elem->screen_pos.x - (slider->button.pos.w / 2), 0));
	else
		return ;
	slider->button.pos.x = ft_clamp(slider->button.pos.x, 0, elem->pos.w - slider->button.pos.w);
	slider->value = ui_get_slider_value(slider->min_value, slider->max_value, slider->button.pos.x, elem->pos.w - slider->button.pos.w);
	slider->update = 1;
}

int	ui_slider_render(t_ui_element *elem)
{
	char		temp[20];
	t_ui_slider	*slider;
	t_ui_button	*button;

	slider = elem->element;
	button = slider->button.element;

	if (!ui_element_render(elem))
		return (0);
	if (slider->update)
	{
		ft_b_itoa(slider->value, temp);
		ui_label_text_set(&button->label, temp);
		slider->update = 0;
	}
	ui_button_render(&slider->button);
	return (1);
}

void	ui_slider_free(void *elem)
{
	(void)elem;
}
