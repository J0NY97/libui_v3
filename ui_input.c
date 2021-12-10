#include "libui.h"

void	ui_input_new(t_ui_window *win, t_ui_element *elem)
{
	t_ui_input	*input;

	ui_element_new(win, elem);
	input = ft_memalloc(sizeof(t_ui_input));
	input->elem = elem;
	elem->element = input;
	elem->element_type = UI_TYPE_INPUT;
	ui_label_new(win, &input->label);
	ui_label_set_text(&input->label, "Input Text");
	ui_element_set_parent(&input->label, elem, UI_TYPE_ELEMENT);
	ui_label_new(win, &input->placeholder);
	ui_label_set_text(&input->placeholder, "Placeholder");
	ui_element_set_parent(&input->placeholder, elem, UI_TYPE_ELEMENT);
	input->label.render_me_on_parent = 1;
	input->placeholder.render_me_on_parent = 1;
}

void	ui_input_edit(t_ui_element *elem, t_ui_recipe *recipe)
{
	t_ui_input	*input;

	input = elem->element;
	ui_label_edit(&input->label, recipe);
	ui_label_edit(&input->placeholder, recipe);
	if (recipe->placeholder_text)
	{
		ui_label_set_text(&input->placeholder, recipe->placeholder_text);
		ui_label_color_set(&input->placeholder,
			ui_color_change_brightness(
				ui_label_get_label(&input->label)->font_color, -0.5));
	}
	input->input_type = recipe->input_type;
}

void	remove_str_from_n_to_m(char **dest, int n, int m)
{
	char	*temp0;
	char	*temp1;
	int		len;

	len = ft_strlen(*dest);
	temp0 = ft_strsub(*dest, 0, n);
	temp1 = ft_strsub(*dest, m, len);
	ft_strdel(dest);
	*dest = ft_strjoin(temp0, temp1);
	ft_strdel(&temp0);
	ft_strdel(&temp1);
}

void	remove_char_after_nth_char(char **dest, int n)
{
	char	*temp0;
	char	*temp1;
	int		len;

	len = ft_strlen(*dest);
	temp0 = ft_strsub(*dest, 0, n - 1);
	temp1 = ft_strsub(*dest, n, len);
	ft_strdel(dest);
	*dest = ft_strjoin(temp0, temp1);
	ft_strdel(&temp0);
	ft_strdel(&temp1);
}

void	insert_str_after_nth_char(char **dest, char *src, int n)
{
	char	*temp0;
	char	*temp1;
	int		len;

	len = ft_strlen(*dest);
	temp0 = ft_strsub(*dest, 0, n);
	temp1 = ft_strsub(*dest, n, len);
	ft_stradd(&temp0, src);
	ft_stradd(&temp0, temp1);
	ft_strdel(dest);
	*dest = ft_strdup(temp0);
	ft_strdel(&temp0);
	ft_strdel(&temp1);
}

char	*str_remove_all_numbers(char *str)
{
	char	*final;
	char	temp[256];
	int		i;
	int		j;

	i = -1;
	j = -1;
	while (str[++i])
		if (!ft_isdigit(str[i]))
			temp[++j] = str[i];
	temp[++j] = '\0';
	final = ft_strdup(temp);
	return (final);
}

char	*str_remove_all_letters(char *str)
{
	char	*final;
	char	temp[256];
	int		i;
	int		j;

	i = -1;
	j = -1;
	while (str[++i])
		if (!ft_isalpha(str[i]))
			temp[++j] = str[i];
	temp[++j] = '\0';
	final = ft_strdup(temp);
	return (final);
}

void	ui_input_remove_selected(t_ui_element *elem)
{
	t_ui_input	*input;
	t_ui_label	*label;
	int			small;
	int			big;

	input = elem->element;
	label = ui_input_get_label(elem);
	small = ft_min(input->cursor_on_char_num, input->cursor_from_char_num);
	big = ft_max(input->cursor_on_char_num, input->cursor_from_char_num);
	if (big - small > 0)
	{
		remove_str_from_n_to_m(&label->text, small, big);
		input->cursor_on_char_num = small;
		input->cursor_from_char_num = input->cursor_on_char_num;
	}
}

void	ui_input_add(t_ui_element *elem, char *str)
{
	t_ui_input	*input;
	t_ui_label	*label;
	char		*temp;

	input = elem->element;
	label = ui_input_get_label(elem);
	ui_input_remove_selected(elem);
	if (input->input_type == 1)
		temp = str_remove_all_letters(str);
	else if (input->input_type == 2)
		temp = str_remove_all_numbers(str);
	else
		temp = ft_strdup(str);
	insert_str_after_nth_char(&label->text, temp, input->cursor_on_char_num);
	input->cursor_on_char_num += ft_strlen(temp);
	input->cursor_from_char_num = input->cursor_on_char_num;
	ft_strdel(&temp);
}

void	ui_input_decide_clickiness(
	t_ui_element *elem, t_ui_input *input, SDL_Event e)
{
	static unsigned int	text_started = 0;

	if (elem->is_hover && elem->win->mouse_down)
	{
		if (!elem->is_click)
		{
			SDL_StartTextInput();
			text_started++;
		}
		elem->is_click = 1;
	}
	else if ((!elem->is_hover && elem->win->mouse_down)
		|| (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN))
	{
		text_started--;
		if (text_started == 0)
			SDL_StopTextInput();
		elem->is_click = 0;
		input->input_exit = 1;
	}
}

void	ui_input_event(t_ui_element *elem, SDL_Event e)
{
	t_ui_input			*input;

	input = elem->element;
	if (!elem->rendered_last_frame || !elem->event)
		return ;
	elem->is_hover = ui_element_is_hover(elem);
	ui_input_decide_clickiness(elem, input, e);
	if (elem->is_click)
		ui_input(elem, e);
}

void	ui_input_get_small_n_big(t_ui_input *input, int *small, int *big)
{
	*small = ft_min(input->cursor_on_char_num, input->cursor_from_char_num);
	*big = ft_max(input->cursor_on_char_num, input->cursor_from_char_num);
}

void	ui_input_ctrl_events(
	t_ui_element *elem, t_ui_input *input, t_ui_label *label, SDL_Event e)
{
	char	*temp;
	int		small;
	int		big;

	small = ft_min(input->cursor_on_char_num, input->cursor_from_char_num);
	big = ft_max(input->cursor_on_char_num, input->cursor_from_char_num);
	if (e.key.keysym.sym == SDLK_c || e.key.keysym.sym == SDLK_x)
	{
		temp = ft_strsub(label->text, small, big - small);
		SDL_SetClipboardText(temp);
		ft_strdel(&temp);
		if (e.key.keysym.sym == SDLK_x)
			ui_input_remove_selected(elem);
	}
	if (e.key.keysym.sym == SDLK_v)
	{
		temp = SDL_GetClipboardText();
		ui_input_add(elem, temp);
		SDL_free(temp);
	}
	if (e.key.keysym.sym == SDLK_a)
	{
		input->cursor_from_char_num = 0;
		input->cursor_on_char_num = ft_strlen(label->text);
	}
}

void	ui_input_remove(t_ui_input *input, t_ui_label *label, SDL_Event e)
{
	int	small;
	int	big;

	small = ft_min(input->cursor_on_char_num, input->cursor_from_char_num);
	big = ft_max(input->cursor_on_char_num, input->cursor_from_char_num);
	if (big - small == 0)
	{
		if (e.key.keysym.sym == SDLK_BACKSPACE)
			remove_str_from_n_to_m(&label->text, small - 1, small);
		else
			remove_str_from_n_to_m(&label->text, small, small + 1);
	}
	else
		remove_str_from_n_to_m(&label->text, small, big);
}

void	ui_input_key_events(t_ui_element *elem, SDL_Event e)
{
	t_ui_input	*input;
	t_ui_label	*label;
	char		*temp;
	int			len;

	input = elem->element;
	label = ui_input_get_label(elem);
	len = ft_strlen(label->text);
	if (KMOD_LCTRL & SDL_GetModState())
		ui_input_ctrl_events(elem, input, label, e);
	else if (e.key.keysym.sym == SDLK_END)
		input->cursor_on_char_num = len;
	else if (e.key.keysym.sym == SDLK_HOME)
		input->cursor_on_char_num = 0;
	else if (e.key.keysym.sym == SDLK_LEFT)
		input->cursor_on_char_num--;
	else if (e.key.keysym.sym == SDLK_RIGHT)
		input->cursor_on_char_num++;
	else if (e.key.keysym.sym == SDLK_BACKSPACE
		|| e.key.keysym.sym == SDLK_DELETE)
		ui_input_remove(input, label, e);
	else if (!(KMOD_LSHIFT & SDL_GetModState()))
		ui_input_remove_selected(elem);
}

void	ui_input_mouse_events(
	t_ui_element *elem, t_ui_input *input, t_ui_label *lbl, SDL_Event e)
{
	int	i;

	e.button.clicks %= 4;
	if (e.button.clicks == 1)
	{
		input->cursor_on_char_num = get_nth_char_of_text_at_x(lbl->text,
				elem->win->mouse_pos.x - input->label.screen_pos.x, lbl->font);
		input->cursor_from_char_num = input->cursor_on_char_num;
	}
	else if (e.button.clicks == 2)
	{
		i = input->cursor_on_char_num;
		while (lbl->text[--i] && !ft_isspace(lbl->text[i]))
			;
		input->cursor_from_char_num = ft_clamp(++i, 0, ft_strlen(lbl->text));
		while (lbl->text[++i] && !ft_isspace(lbl->text[i]))
			;
		input->cursor_on_char_num = ft_clamp(i, 0, ft_strlen(lbl->text));
	}
	else if (e.button.clicks == 3)
	{
		input->cursor_from_char_num = 0;
		input->cursor_on_char_num = ft_strlen(lbl->text);
	}
}

/*
 * IMPORTANT: Remember that when you have pressed lshift,
 *		it will not reset the from_char_num...
 *		 ...so whatever you do to this dont change that.!
 *
 * Dont unselect if you have pressed control or
 *	if you have typed a capital letter by holding shift;
 *
 *  int	text_input_shift_pressed;
 * 		is for capital chars. we dont want the selection to automatically
 * 		select the just typed capital letter and then you have to unselect
 * 		it manually before continuing your typing....
*/
void	ui_input_check_if_unselect(t_ui_element *elem, SDL_Event e)
{
	t_ui_input	*input;
	int			text_input_key_pressed;
	int			text_input_shift_pressed;

	input = elem->element;
	text_input_key_pressed = e.type == SDL_TEXTINPUT || e.type == SDL_KEYDOWN;
	text_input_shift_pressed = e.type == SDL_TEXTINPUT
		&& (KMOD_LSHIFT & SDL_GetModState());
	if (text_input_key_pressed && !(KMOD_LCTRL & SDL_GetModState())
		&& (text_input_shift_pressed || !(KMOD_LSHIFT & SDL_GetModState())))
		input->cursor_from_char_num = input->cursor_on_char_num;
}

void	ui_input(t_ui_element *elem, SDL_Event e)
{
	t_ui_input	*input;
	t_ui_label	*lbl;
	int			len;

	input = elem->element;
	lbl = ui_input_get_label(elem);
	if (e.type == SDL_TEXTINPUT)
		ui_input_add(elem, e.text.text);
	else if (e.type == SDL_KEYDOWN)
		ui_input_key_events(elem, e);
	if (e.type == SDL_MOUSEBUTTONDOWN)
		ui_input_mouse_events(elem, input, lbl, e);
	else if (elem->is_hover && elem->win->mouse_down == SDL_BUTTON_LEFT
		&& e.type == SDL_MOUSEMOTION)
	{
		if (elem->win->mouse_down_last_frame)
			input->cursor_from_char_num = input->cursor_on_char_num;
		input->cursor_on_char_num = get_nth_char_of_text_at_x(lbl->text,
				elem->win->mouse_pos.x - input->label.screen_pos.x, lbl->font);
	}
	lbl->texture_recreate = 1;
	len = ft_strlen(lbl->text);
	input->cursor_on_char_num = ft_clamp(input->cursor_on_char_num, 0, len);
	input->cursor_from_char_num = ft_clamp(input->cursor_from_char_num, 0, len);
	ui_input_check_if_unselect(elem, e);
}

void	ui_input_render_highlight(t_ui_element *elem)
{
	t_vec2i		pos_on;
	t_vec2i		pos_from;
	t_ui_input	*input;
	t_ui_label	*label;
	t_vec2i		w;

	input = elem->element;
	label = ui_input_get_label(elem);
	pos_on = vec2i(input->cursor_on_char_x + elem->screen_pos.x,
			elem->screen_pos.y + 2);
	pos_from = vec2i(input->cursor_from_char_x + elem->screen_pos.x,
			elem->screen_pos.y + 2);
	SDL_SetRenderTarget(elem->win->renderer, elem->win->texture);
	SDL_SetRenderDrawColor(elem->win->renderer, 255, 0, 0, 255);
	SDL_RenderDrawLine(elem->win->renderer,
		pos_on.x, pos_on.y, pos_on.x, pos_on.y + elem->screen_pos.h - 4);
	SDL_SetRenderDrawColor(elem->win->renderer, 0, 0, 255, 255);
	SDL_RenderDrawLine(elem->win->renderer, pos_from.x, pos_from.y,
		pos_from.x, pos_from.y + elem->screen_pos.h - 4);
	SDL_SetRenderDrawColor(elem->win->renderer, 0, 0, 255, 255);
	w.x = ft_max(pos_from.x, pos_on.x);
	w.y = ft_min(pos_from.x, pos_on.x);
	SDL_RenderFillRect(elem->win->renderer,
		&(SDL_Rect){w.y, pos_from.y, w.x - w.y, elem->screen_pos.h - 4});
	SDL_SetRenderTarget(elem->win->renderer, NULL);
}

int	ui_input_render(t_ui_element *elem)
{
	t_ui_input	*input;
	t_ui_label	*label;

	input = elem->element;
	label = input->label.element;
	if (!ui_element_render(elem))
		return (0);
	input->input_exit = 0;
	if (label->text_wh.x > 0 || elem->is_click)
		input->placeholder.show = 0;
	else
		input->placeholder.show = 1;
	if (elem->is_click)
	{
		input->cursor_on_char_x = get_x_of_char_in_text(label->text,
				input->cursor_on_char_num, label->font) + input->label.pos.x;
		input->cursor_from_char_x = get_x_of_char_in_text(label->text,
				input->cursor_from_char_num, label->font) + input->label.pos.x;
		ui_input_render_highlight(elem);
	}
	ui_label_render(&input->label);
	ui_label_render(&input->placeholder);
	return (1);
}

void	ui_input_free(void *elem, size_t size)
{
	t_ui_element	*element;
	t_ui_input		*input;

	element = elem;
	if (!element)
		return ;
	input = element->element;
	if (!input)
		return ;
	free(input);
	(void)size;
}

void	ui_input_print(t_ui_element *elem)
{
	t_ui_input	*input;

	if (elem->element_type != UI_TYPE_INPUT)
		return ;
	input = elem->element;
	ui_label_print(&input->label);
	ui_label_print(&input->placeholder);
}

t_ui_input	*ui_input_get(t_ui_element *elem)
{
	if (elem->element_type != UI_TYPE_INPUT)
	{
		ft_printf("[%s] Elem not of type UI_TYPE_INPUT. %d %s.\n",
			__FUNCTION__, elem->element_type,
			ui_element_type_to_string(elem->element_type));
		return (NULL);
	}
	return (elem->element);
}

t_ui_element	*ui_input_get_label_element(t_ui_element *elem)
{
	if (!elem)
		return (NULL);
	return (&((t_ui_input *)elem->element)->label);
}

t_ui_label	*ui_input_get_label(t_ui_element *elem)
{
	return (ui_input_get_label_element(elem)->element);
}

char	*ui_input_get_text(t_ui_element *elem)
{
	t_ui_input	*input;
	t_ui_label	*label;

	if (!elem)
		return (NULL);
	input = elem->element;
	if (!input)
		return (NULL);
	label = input->label.element;
	if (!label)
		return (NULL);
	return (label->text);
}

void	ui_input_set_text(t_ui_element *elem, char *str)
{
	ui_label_set_text(ui_input_get_label_element(elem), str);
}

void	ui_input_set_placeholder_text(t_ui_element *elem, char *str)
{
	ui_label_set_text(&ui_input_get(elem)->placeholder, str);
}

int	ui_input_exit(t_ui_element *elem)
{
	return (((t_ui_input *)elem->element)->input_exit);
}
