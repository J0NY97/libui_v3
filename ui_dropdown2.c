#include "libui.h"

/*
 * This will return 0/1 every frame.
*/
int	ui_dropdown_is_open(t_ui_element *elem)
{
	if (elem->element_type != UI_TYPE_DROPDOWN)
	{
		ft_printf("[%s] Elem is not of type UI_TYPE_DROPDOWN.[%s, %s]\n",
			__FUNCTION__, elem->id,
			ui_element_type_to_string(elem->element_type));
		return (-1);
	}
	return (((t_ui_dropdown *)elem->element)->menu.show);
}

/*
 * This will only return 1 the frame the menu was opened.
*/
int	ui_dropdown_open(t_ui_element *elem)
{
	if (elem->element_type != UI_TYPE_DROPDOWN)
	{
		ft_printf("[%s] Elem is not of type UI_TYPE_DROPDOWN.[%s, %s]\n",
			__FUNCTION__, elem->id,
			ui_element_type_to_string(elem->element_type));
		return (-1);
	}
	return (((t_ui_dropdown *)elem->element)->drop_open);
}

int	ui_dropdown_exit(t_ui_element *elem)
{
	if (elem->element_type != UI_TYPE_DROPDOWN)
	{
		ft_printf("[%s] Elem is not of type UI_TYPE_DROPDOWN.[%s, %s]\n",
			__FUNCTION__, elem->id,
			ui_element_type_to_string(elem->element_type));
		return (-1);
	}
	return (((t_ui_dropdown *)elem->element)->drop_exit);
}

char	*ui_dropdown_active_text(t_ui_element *elem)
{
	t_ui_element	*active;
	t_ui_label		*label;

	if (!elem)
		return (NULL);
	active = ui_dropdown_get_dropdown(elem)->active;
	if (!active)
		return (NULL);
	label = ui_button_get_label(active);
	if (!label)
		return (NULL);
	return (label->text);
}

t_ui_element	*ui_dropdown_active(t_ui_element *elem)
{
	t_ui_dropdown	*drop;

	if (!elem)
		return (NULL);
	if (elem->element_type != UI_TYPE_DROPDOWN)
	{
		ft_printf("[%s] Elem not of type UI_TYPE_DROPDOWN.%d %s.\n",
			__FUNCTION__, elem->element_type,
			ui_element_type_to_string(elem->element_type));
		return (NULL);
	}
	return (ui_dropdown_get_dropdown(elem)->active);
}
