#include "libui_v3.h"

void	ui_input_new(t_ui_window *win, t_ui_element *elem)
{
	t_ui_input	*input;

	ui_element_new(win, elem);
	input = ft_memalloc(sizeof(t_ui_input));
	input->elem = elem;
	elem->element = input;
	elem->element_type = UI_TYPE_INPUT;
	ui_label_new(win, &input->label);
}

void	ui_input_event(t_ui_element *elem, SDL_Event e)
{
	t_ui_input	*input;
	t_ui_label	label;
	int			len;

	input = elem->element;
	label = input->label.element;

	elem->is_hover = 0;
	if (point_in_rect(elem->win->mouse_pos, elem->screen_pos))
		elem->is_hover = 1;
	else if (elem->is_click == 0)
		return ;
	len = ft_strlen(label.text);
	if (elem->is_hover == 1 && e.type == SDL_MOUSEBUTTONDOWN)
	{
		if (elem->is_click == 0)
		{
			SDL_StartTextInput();
			input->cursor_on_char_num = ft_strlen(label.text);
			input->cursor_from_char_num = input->cursor_on_char_num;
		}
		elem->is_click = 1;
	}
	if (elem->is_click == 1)
	{
		if (e.type == SDL_TEXTINPUT)
		{
			insert_str_after_nth_char(&label.text, e.text.text, input->cursor_on_char_num);
			input->cursor_on_char_num++;
		}
		else if (e.type == SDL_KEYDOWN)
		{
			if (KMOD_LCTRL & SDL_GetModState()) // All the modstate thingys should probably come first of all. Because some of them use other keys that are in use without the modstate.
			{
				// This needs to gtfo too.
				int small = ft_min(input->cursor_on_char_num, input->cursor_from_char_num);
				int big = ft_max(input->cursor_on_char_num, input->cursor_from_char_num);
				if (e.key.keysym.sym == SDLK_c)
				{
					char *clipboard;

					clipboard = ft_strsub(label.text, small, big - small);
					SDL_SetClipboardText(clipboard);
					ft_strdel(&clipboard);
				}
				else if (e.key.keysym.sym == SDLK_v)
				{
					char *clipboard;
					clipboard = SDL_GetClipboardText();

					// This needs to gtfo
					if (big - small > 0)
					{
						remove_str_from_n_to_m(&label.text, small, big);
						input->cursor_on_char_num = small;
					}

					insert_str_after_nth_char(&label.text, clipboard, input->cursor_on_char_num);
					input->cursor_on_char_num += ft_strlen(clipboard);
					SDL_free(clipboard);
					input->cursor_from_char_num = input->cursor_on_char_num;
				}
				else if (e.key.keysym.sym == SDLK_a)
				{
					input->cursor_from_char_num = 0;
					input->cursor_on_char_num = len;
				}
				else if (e.key.keysym.sym == SDLK_x)
				{
					char *clipboard;

					clipboard = ft_strsub(label.text, small, big - small);
					SDL_SetClipboardText(clipboard);
					ft_strdel(&clipboard);
					// This needs to gtfo
					if (big - small > 0)
					{
						remove_str_from_n_to_m(&label.text, small, big);
						input->cursor_on_char_num = small;
						input->cursor_from_char_num = small;
					}
				}
			}
			else if (e.key.keysym.sym == SDLK_END)
			{
				input->cursor_on_char_num = len;
			}
			else if (e.key.keysym.sym == SDLK_HOME)
			{
				input->cursor_on_char_num = 0;
			}
			else if (e.key.keysym.sym == SDLK_RETURN)
			{
				SDL_StopTextInput();
				elem->is_click = 0;
				return ;
			}
			else if (e.key.keysym.sym == SDLK_LEFT)
			{
				input->cursor_on_char_num--;
			}
			else if (e.key.keysym.sym == SDLK_RIGHT)
			{
				input->cursor_on_char_num++;
			}
			else if (e.key.keysym.sym == SDLK_BACKSPACE || e.key.keysym.sym == SDLK_DELETE)
			{
				int small = ft_min(input->cursor_on_char_num, input->cursor_from_char_num);
				int big = ft_max(input->cursor_on_char_num, input->cursor_from_char_num);
				if (big - small == 0)
				{
					if (e.key.keysym.sym == SDLK_BACKSPACE)
					{
						if (len - 1 >= 0 && input->cursor_on_char_num - 1 >= 0)
						{
							remove_str_from_n_to_m(&label.text, input->cursor_on_char_num - 1, input->cursor_on_char_num);
							input->cursor_on_char_num--;
						}
					}
					else
					{
						if (input->cursor_on_char_num < len)
							remove_char_after_nth_char(&label.text, input->cursor_on_char_num + 1);
					}
				}
				else
				{
					remove_str_from_n_to_m(&label.text, small, big);
					input->cursor_on_char_num = small;
				}
			}
			else 
			{
				// This needs to gtfo
				int small = ft_min(input->cursor_on_char_num, input->cursor_from_char_num);
				int big = ft_max(input->cursor_on_char_num, input->cursor_from_char_num);
				if (big - small > 0)
				{
					remove_str_from_n_to_m(&label.text, small, big);
					input->cursor_on_char_num = small;
				}
			}
		} // END of e->type == SDL_KEYDOWN
		if(e.type == SDL_MOUSEBUTTONDOWN)
		{
			if (elem->is_hover != 1)
			{
				elem->is_click = 0;
				//SDL_StopTextInput();
				return ;
			}
			// clicks == 1, move the cursor there
			// clicks == 2, select first word at cursor
			// clicks == 3, select whole text
			e.button.clicks %= 4; // small hackeroni, to cycle all the clicks.
			if (e.button.clicks == 1) // if one click, move the cursor to that point.
			{
				if (elem->is_hover == 1)
				{
					// Dont need y because that doesnt matter. only needed if the text can stack on top of eachother, think about it, hard to explain.
					int real_x = e.button.x - input->label.pos.x;
					int total_w = 0;
					int w = 0;
					int h = 0;
					int i = 0;
					char *str = ft_strnew(1);
					while (total_w < real_x) // figure out how tarkka this should be.
					{
						str[0] = label.text[i];
						TTF_SizeText(label.font, str, &w, &h);
						total_w += w;
						i++;
					}
					ft_strdel(&str);
					input->cursor_on_char_num = i;
				}
				input->cursor_from_char_num = input->cursor_on_char_num; // IMPORTANT:make this is in hover != 1 
			}
			else if (e.button.clicks == 2) // select word your cursor is at.
			{
				if (elem->is_hover == 1)
				{
					int i = input->cursor_on_char_num - 1;
					while (label.text[i] && !ft_isspace(label.text[i]))
						i--;
					i++;
					input->cursor_from_char_num = ft_clamp(i, 0, len);
					while (label.text[i] && !ft_isspace(label.text[i]))
						i++;
					input->cursor_on_char_num = ft_clamp(i, 0, len);
				}
			}
			else if (e.button.clicks == 3) // if triple click select the whole text.
			{
				if (elem->is_hover == 1)
				{
					input->cursor_from_char_num = 0;
					input->cursor_on_char_num = len;
				}
			}
		}
		else if ((SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) && e.type == SDL_MOUSEMOTION)
		{
			if (elem->is_hover == 1)
			{
				if (e.button.state == SDL_PRESSED)
					input->cursor_from_char_num = input->cursor_on_char_num;
				int real_x = e.button.x - input->label.pos.x;
				int total_w = 0;
				int w = 0;
				int h = 0;
				int i = 0;
				char *str = ft_strnew(1);
				while (total_w < real_x) // figure out how tarkka this should be.
				{
					str[0] = label.text[i];
					TTF_SizeText(label.font, str, &w, &h);
					total_w += w;
					i++;
				}
				ft_strdel(&str);
				input->cursor_on_char_num = i;
			}
		}
		len = ft_strlen(label.text);
		input->cursor_on_char_num = ft_clamp(input->cursor_on_char_num, 0, len);
		input->cursor_from_char_num = ft_clamp(input->cursor_from_char_num, 0, len);
		// Rethink this trash
		// IMPORTANT: Remember that when you have lshift in the bottom, it will not reset the from_char_num... so whatever you do to this dont change that.!
		int too_long_0 = e.type == SDL_TEXTINPUT || e.type == SDL_KEYDOWN;
		int too_long_1 = e.type == SDL_TEXTINPUT && (KMOD_LSHIFT & SDL_GetModState()); // this is for capital chars. we dont want the selection to automatically select the just typed capital letter and then you have to unselect it manually before continuing your typing....
		// Reset selection if
		if (too_long_0 && !(KMOD_LCTRL & SDL_GetModState()) && (too_long_1 || !(KMOD_LSHIFT & SDL_GetModState())))
		{
			input->cursor_from_char_num = input->cursor_on_char_num;
		}
	}
	len = ft_strlen(label.text);
	input->cursor_on_char_num = ft_clamp(input->cursor_on_char_num, 0, len);
	input->cursor_from_char_num = ft_clamp(input->cursor_from_char_num, 0, len);
}

void	ui_input_render(t_ui_element *elem)
{
	t_ui_input	*input;

	input = elem->element;
	ui_element_render(elem);
	ui_label_render(&input->label);
}

void	ui_input_free(void *elem)
{
	(void)elem;
}

void	ui_input_get(t_ui_get *get)
{
	(void)get;
}
