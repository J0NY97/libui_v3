#include "libui_v3.h"

void	ui_slider_new(t_ui_window *win, t_ui_element *elem)
{
	t_ui_slider	*slider;
	t_ui_button	*button;
	char		temp[20];

	ui_element_new(win, elem);
	elem->element = ft_memalloc(sizeof(t_ui_slider));
	elem->element_type = UI_TYPE_SLIDER;
	slider = elem->element;
	slider->elem = elem;
	ui_button_new(win, &slider->button);
	button = slider->button.element;
	slider->value = 50;
	slider->min_value = 0;
	slider->max_value = 100;
}

void	ui_slider_event(t_ui_element *elem, SDL_Event e)
{
	t_ui_slider	*slider;
	t_ui_button	*button;

	slider = elem->element;
	button = slider->button.element;
	ui_button_event(&slider->button, e);
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

	ui_label_text_set(&slider->min_label, ft_b_itoa(slider->min_value, temp));
	ui_label_text_set(&slider->max_label, ft_b_itoa(slider->max_value, temp));
	ui_label_text_set(&button->label, ft_b_itoa(slider->value, temp));

	ui_button_render(&slider->button);
	ui_label_render(&slider->min_label);
	ui_label_render(&slider->max_label);
	return (1);
}

void	ui_slider_free(void *elem)
{
	(void)free;
}
