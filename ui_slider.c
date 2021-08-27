#include "libui_v3.h"

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
	ui_element_parent_set(&slider->button, elem, UI_TYPE_ELEMENT, &elem->show);
	button = slider->button.element;

	slider->value = 50;
	slider->min_value = 0;
	slider->max_value = 100;
	ui_label_new(win, &slider->min_label);
	ui_label_new(win, &slider->max_label);
	ui_element_pos_set(&slider->min_label, vec4(-10, 0, elem->pos.h, elem->pos.h));
	ui_element_pos_set(&slider->max_label, vec4(elem->pos.w, 0, elem->pos.h, elem->pos.h));
	ui_element_parent_set(&slider->min_label, elem, UI_TYPE_ELEMENT, &elem->show);
	ui_element_parent_set(&slider->max_label, elem, UI_TYPE_ELEMENT, &elem->show);
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

	ft_b_itoa(slider->min_value, temp);
	ui_label_text_set(&slider->min_label, temp);
	ft_b_itoa(slider->max_value, temp);
	ui_label_text_set(&slider->max_label, temp);
	ft_b_itoa(slider->value, temp);
	ui_label_text_set(&button->label, temp);

	ui_element_pos_set2(&slider->min_label, vec2(-10, 0));
	ui_element_pos_set2(&slider->max_label, vec2(elem->pos.w, 0));

	ui_button_render(&slider->button);
	ui_label_render(&slider->min_label);
	ui_label_render(&slider->max_label);
	return (1);
}

void	ui_slider_free(void *elem)
{
	(void)elem;
}
