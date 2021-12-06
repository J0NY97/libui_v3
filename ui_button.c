#include "libui.h"

void	ui_button_new(t_ui_window *win, t_ui_element *elem)
{
	t_ui_button	*button;

	ui_element_new(win, elem);
	elem->element = ft_memalloc(sizeof(t_ui_button));
	elem->element_type = UI_TYPE_BUTTON;
	button = elem->element;
	button->elem = elem;
	ui_element_color_set(elem, UI_STATE_DEFAULT, 0xff95d7ae);
	ui_element_color_set(elem, UI_STATE_HOVER, 0xff7bae7f);
	ui_element_color_set(elem, UI_STATE_CLICK, 0xff73956f);
	ui_label_new(win, &button->label);
	button->label.is_a_part_of_another = 1;
	ui_element_set_parent(&button->label, elem, UI_TYPE_ELEMENT);
	ui_label_text_align(&button->label, UI_TEXT_ALIGN_CENTER);
}

void	ui_button_edit(t_ui_element *elem, t_ui_recipe *recipe)
{
	t_ui_button	*button;

	button = elem->element;
	ui_label_edit(&button->label, recipe);
}

void	ui_button_event(t_ui_element *elem, SDL_Event e)
{
	if (!elem->show || !elem->event)
		return ;
	elem->is_hover = ui_element_is_hover(elem);
	if (elem->is_hover && elem->win->mouse_down)
		elem->is_click = 1;
	if (elem->is_click && !elem->win->mouse_down)
		elem->is_click = 0;
	if (elem->is_click)
		elem->state = UI_STATE_CLICK;
	else if (elem->is_hover)
		elem->state = UI_STATE_HOVER;
	else
		elem->state = UI_STATE_DEFAULT;
	elem->was_click = elem->is_click;
	if (elem->is_toggle == 1)
		elem->state = UI_STATE_CLICK;
}

bool	ui_button(t_ui_element *elem)
{
	if (elem->is_click)
	{
		elem->is_click = 0;
		elem->is_hover = 0;
		elem->state = UI_STATE_CLICK;
		return (1);
	}
	return (0);
}

/*
 * NOTE: !!!!!!!!!!!! Dont do anything that needs the element type to be UI_TYPE_BUTTOn
 * since our dropdown is using this button_redner for reason.!!!!!!!!!!!!!!!!!!!!!!!!!!
*/
int	ui_button_render(t_ui_element *elem)
{
	t_ui_button	*button;
	t_ui_label	*label;

	if (!ui_element_render(elem))
		return (0);
	button = elem->element;
	label = button->label.element;
	ui_label_render(&button->label);
	ui_label_text_align(&button->label, label->text_align);
	return (1);
}

/*
 * Returns from button element its child elements
 * 	(that are specific to this type of element only);
*/
t_ui_element	*ui_button_get(t_ui_element *elem, int ui_type)
{
	if (elem->element_type != UI_TYPE_BUTTON)
		return (NULL);
	if (ui_type == UI_TYPE_LABEL)
		return (ui_button_get_label_element(elem));
	return (NULL);
}

t_ui_button	*ui_button_get_button(t_ui_element *elem)
{
	if (elem->element_type != UI_TYPE_BUTTON)
	{
		ft_printf("[%s] Elem is not of type button. <%d : %s>\n",
			__FUNCTION__, elem->element_type,
			ui_element_type_to_string(elem->element_type));
		return (NULL);
	}
	return (elem->element);
}

void	ui_button_free(void *elem, size_t size)
{
	t_ui_element	*element;
	t_ui_button		*button;

	element = elem;
	if (!elem)
		return ;
	button = element->element;
	if (!button)
		return ;
	//ui_element_free(&button->label, UI_TYPE_LABEL);
	free(button);
	(void)size;
}

void	ui_button_print(t_ui_element *elem)
{
	t_ui_button	*button;

	if (elem->element_type != UI_TYPE_BUTTON)
	{
		ft_printf("[%s] Not elem of type button. <%d : %s>",
			__FUNCTION__, elem->element_type,
			ui_element_type_to_string(elem->element_type));
		return ;
	}
	button = ui_button_get_button(elem);
	if (!button)
		return ;
	ui_element_print(&button->label);
}

/*
 * Getters
*/
t_ui_element	*ui_button_get_label_element(t_ui_element *elem)
{
	t_ui_button	*button;

	if (!elem)
	{
		ft_printf("[%s] No element.\n", __FUNCTION__);
		return (NULL);
	}
	if (elem->element_type != UI_TYPE_BUTTON)
	{
		ft_printf("[%s] Element not of type UI_TYPE_BUTTON. It\'s %d %s.",
			__FUNCTION__, elem->element_type,
			ui_element_type_to_string(elem->element_type));
		return (NULL);
	}
	button = elem->element;
	if (!button)
		return (NULL);
	return (&button->label);
}

t_ui_label	*ui_button_get_label(t_ui_element *elem)
{
	t_ui_element	*label;

	label = ui_button_get_label_element(elem);
	if (!label)
		return (NULL);
	return (label->element);
}

void	ui_button_set_text(t_ui_element *elem, char *str)
{
	t_ui_element		*label;

	label = ui_button_get_label_element(elem);
	ui_label_set_text(label, str);
}

char	*ui_button_get_text(t_ui_element *elem)
{
	t_ui_element		*label;

	label = ui_button_get_label_element(elem);
	return (ui_label_get_text(label));
}

/*
 * From list get the button with str as text;
*/
t_ui_element	*ui_list_get_button_with_text(t_list *list, const char *str)
{
	while (list)
	{
		if (ft_strequ(ui_button_get_text(list->content), str))
			return (list->content);
		list = list->next;
	}
	ft_printf("[%s] No button with text %s in list.\n", __FUNCTION__, str);
	return (NULL);
}
