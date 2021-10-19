#include "libui.h"

void	ui_scrollbar_new(t_ui_window *win, t_ui_element *elem)
{
	t_ui_scrollbar	*scroll;

	ui_element_new(win, elem);
	elem->element = ft_memalloc(sizeof(t_ui_scrollbar));
	elem->element_type = UI_TYPE_SCROLLBAR;

	scroll = elem->element;
	ui_button_new(win, &scroll->button);
	ui_element_parent_set(&scroll->button, elem, UI_TYPE_ELEMENT);

	ui_element_pos_set(&scroll->button, vec4(0, 0, elem->pos.h, elem->pos.h));
	ui_element_color_set(&scroll->button, UI_STATE_DEFAULT, UI_COLOR_ORANGEISH);
	ui_element_color_set(&scroll->button, UI_STATE_HOVER, UI_COLOR_ORANGEISH_DARKER);
	ui_element_color_set(&scroll->button, UI_STATE_CLICK, UI_COLOR_ORANGEISH_DARKEST);
	ui_element_parent_set(&scroll->button, elem, UI_TYPE_ELEMENT);

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
		scroll->value = ui_get_slider_value(scroll->min, scroll->max, elem->win->mouse_pos.y - elem->screen_pos.y, elem->pos.h - (scroll->button.pos.h / 2));
		scroll->update = 1;
	}
}

int	ui_scrollbar_render(t_ui_element *elem)
{
	char		temp[20];
	t_ui_scrollbar	*scroll;
	t_ui_button	*button;

	scroll = elem->element;
	button = scroll->button.element;

	if (!ui_element_render(elem))
		return (0);
	if (scroll->update)
	{
		scroll->button.pos.y = ft_clamp(ui_set_slider_value(scroll->value, scroll->min, scroll->max, elem->pos.h - scroll->button.pos.h), 0, elem->pos.h - scroll->button.pos.h);
		ft_b_itoa(scroll->value, temp);
		ui_label_text_set(&button->label, temp);
		scroll->update = 0;
	}
	ui_button_render(&scroll->button);
	return (1);
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


