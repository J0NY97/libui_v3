#include "libui.h"

void	ui_window_new(t_ui_window *win, char *title, t_vec4 pos)
{
	memset(win, 0, sizeof(t_ui_window));
	if (!title)
		win->title = ft_strdup("Window Title Not Set");
	else
		win->title = ft_strdup(title);
	win->pos = pos;
	win->win = SDL_CreateWindow(win->title,
			win->pos.x, win->pos.y, win->pos.w, win->pos.h, 0);
	win->renderer = SDL_CreateRenderer(win->win, -1, SDL_RENDERER_ACCELERATED);
	win->texture = ui_create_texture(win->renderer, vec2i(win->pos.w, win->pos.h));
	win->show = 1;
	win->children = NULL;
	win->window_id = SDL_GetWindowID(win->win);
	win->screen_pos.w = pos.w;
	win->screen_pos.h = pos.h;
	win->texture_scale = vec2(win->screen_pos.w / win->pos.w,
			win->screen_pos.h / win->pos.h);
	SDL_GetMouseState(&win->window_mouse_pos.x, &win->window_mouse_pos.y);
	win->mouse_pos.x = win->window_mouse_pos.x * win->texture_scale.x;
	win->mouse_pos.y = win->window_mouse_pos.y * win->texture_scale.y;
	win->bg_color = 0xff000000;
	win->free_me = 1;
	win->textures_recreate = 1;
}

void	ui_window_resize_event(t_ui_window *win, SDL_Event e)
{
	t_vec2i	new_pos;

	SDL_GetWindowSize(win->win, &new_pos.x, &new_pos.y);
	win->pos.w = new_pos.x;
	win->pos.h = new_pos.y;
	win->texture_scale = vec2(win->screen_pos.w / win->pos.w,
			win->screen_pos.h / win->pos.h);
	win->textures_recreate = 1;
	ft_printf("[%s] Window resized.\n", __FUNCTION__);
}

void	ui_window_window_events(t_ui_window *win, SDL_Event e)
{
	if (e.window.event == SDL_WINDOWEVENT_RESIZED)
		ui_window_resize_event(win, e);
	else if (e.window.event == SDL_WINDOWEVENT_CLOSE)
	{
		if (win->hide_on_x)
			ui_window_flag_set(win, UI_WINDOW_HIDE);
		else
			win->wants_to_close = 1;
	}
	else if (e.window.event == SDL_WINDOWEVENT_SHOWN)
		win->show = 1;
	else if (e.window.event == SDL_WINDOWEVENT_HIDDEN)
		win->show = 0;
}

void	ui_window_mouse_events(t_ui_window *win, SDL_Event e)
{
	SDL_GetMouseState(&win->window_mouse_pos.x, &win->window_mouse_pos.y);
	win->mouse_pos.x = win->window_mouse_pos.x * win->texture_scale.x;
	win->mouse_pos.y = win->window_mouse_pos.y * win->texture_scale.y;
}

void	ui_window_keydown_events(t_ui_window *win, SDL_Event e)
{
	int	modkey;

	modkey = SDL_GetModState();
	if (modkey & KMOD_LALT)
	{
		if (e.key.keysym.scancode == SDL_SCANCODE_RETURN)
		{
			ui_window_flag_set(win, UI_WINDOW_FULLSCREEN);
			ui_window_resize_event(win, e);
		}
	}
	else if (modkey & KMOD_LCTRL)
	{
		if (e.key.keysym.scancode == SDL_SCANCODE_R)
			win->textures_recreate = 1;
	}
}

void	ui_window_event(t_ui_window *win, SDL_Event e)
{
	if (e.button.type == SDL_MOUSEBUTTONUP)
		win->mouse_down = 0;
	if (win->window_id != e.window.windowID)
		return ;
	if (e.type == SDL_WINDOWEVENT)
		ui_window_window_events(win, e);
	else if (e.type == SDL_MOUSEMOTION)
		ui_window_mouse_events(win, e);
	else if (e.type == SDL_MOUSEWHEEL)
		win->scroll = e.wheel.y;
	else if (e.type == SDL_KEYDOWN)
		ui_window_keydown_events(win, e);
	if (e.button.type == SDL_MOUSEBUTTONDOWN)
	{
		win->mouse_down = e.button.button;
		win->mouse_down_last_frame = win->mouse_down;
	}
}

int	ui_window_render(t_ui_window *win)
{
	t_rgba	rgba;

	SDL_SetRenderTarget(win->renderer, NULL);
	SDL_RenderCopy(win->renderer, win->texture, NULL, NULL);
	SDL_SetRenderTarget(win->renderer, win->texture);
	rgba = hex_to_rgba(win->bg_color);
	SDL_SetRenderDrawColor(win->renderer, rgba.r, rgba.g, rgba.b, rgba.a);
	SDL_RenderClear(win->renderer);
	SDL_SetRenderTarget(win->renderer, NULL);
	win->textures_recreate = 0;
	win->mouse_pos_prev = win->mouse_pos;
	win->scroll = 0;
	win->mouse_down_last_frame = 0;
	SDL_RenderPresent(win->renderer);
	return (1);
}

void	ui_window_free(void *window, size_t size)
{
	t_ui_window	*win;

	win = window;
	if (!win)
		return ;
	ft_strdel(&win->id);
	if (!win->win_replaced)
		SDL_DestroyWindow(win->win);
	if (!win->renderer_replaced)
		SDL_DestroyRenderer(win->renderer);
	SDL_DestroyTexture(win->texture);
	ft_strdel(&win->title);
	ft_printf("title\n");
	ui_list_element_free(&win->children);
	ft_lstdel(&win->children, &dummy_free_er);
	ft_printf("children\n");
	win->layout = NULL;
	if (win->free_me)
		free(win);
	ft_printf("me\n");
}

/*********************************************
 * Window edititing functions down from here.
**********************************************/
void	ui_window_texture_redo(t_ui_window *win)
{
	if (!win)
	{
		ft_printf("[%s] Error : No window.\n", __FUNCTION__);
		return ;
	}
	if (win->texture)
		SDL_DestroyTexture(win->texture);
	win->texture = SDL_CreateTexture(win->renderer, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_TARGET, win->screen_pos.w, win->screen_pos.h);
	win->texture_scale = vec2(win->screen_pos.w / win->pos.w,
			win->screen_pos.h / win->pos.h);
}

/*
 * Change the position of the actual window : win->pos;
*/
void	ui_window_pos_set(t_ui_window *win, t_vec4 pos)
{
	if (win->pos.x != pos.x || win->pos.y != pos.y)
	{
		win->pos.x = pos.x;
		win->pos.y = pos.y;
		SDL_SetWindowPosition(win->win, win->pos.x, win->pos.y);
	}
	if (win->pos.w != pos.w || win->pos.h != pos.h)
	{
		ft_printf("[%s]\n", __FUNCTION__);
		win->pos.w = pos.w;
		win->pos.h = pos.h;
		SDL_SetWindowSize(win->win, win->pos.w, win->pos.h);
		win->texture_scale = vec2(win->screen_pos.w / win->pos.w,
				win->screen_pos.h / win->pos.h);
	}
}

void	ui_window_texture_pos_set(t_ui_window *win, t_vec2i pos)
{
	if (pos.x != win->screen_pos.w || pos.y != win->screen_pos.h)
	{
		win->screen_pos
			= vec4i(win->screen_pos.x, win->screen_pos.y, pos.x, pos.y);
		ui_window_texture_redo(win);
	}
}

int	choose_fullscreen_mode(t_ui_window *win)
{
	int	curr_flags;

	curr_flags = SDL_GetWindowFlags(win->win);
	if (curr_flags & SDL_WINDOW_FULLSCREEN)
		return (0);
	return (SDL_WINDOW_FULLSCREEN);
}

void	ui_window_flag_set(t_ui_window *win, int flags)
{
	if (flags & UI_WINDOW_HIDE)
		SDL_HideWindow(win->win);
	if (flags & UI_WINDOW_SHOW)
		SDL_ShowWindow(win->win);
	if (flags & UI_WINDOW_RAISE)
		SDL_RaiseWindow(win->win);
	if (flags & UI_WINDOW_MAXIMIZE)
		SDL_MaximizeWindow(win->win);
	if (flags & UI_WINDOW_MINIMIZE)
		SDL_MinimizeWindow(win->win);
	if (flags & UI_WINDOW_FULLSCREEN)
		SDL_SetWindowFullscreen(win->win, choose_fullscreen_mode(win));
	if (flags & UI_WINDOW_FULLSCREEN_WINDOWED)
		SDL_SetWindowFullscreen(win->win, SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (flags & UI_WINDOW_GRAB)
		SDL_SetWindowGrab(win->win, SDL_TRUE);
	if (flags & UI_WINDOW_RESIZEABLE)
		SDL_SetWindowResizable(win->win, SDL_TRUE);
}

void	ui_window_title_set(t_ui_window *win, const char *title)
{
	if (win->title)
		ft_strdel(&win->title);
	win->title = ft_strdup(title);
	SDL_SetWindowTitle(win->win, title);
}

void	ui_window_id_set(t_ui_window *win, const char *id)
{
	if (!id)
		return ;
	if (win->id)
		ft_strdel(&win->id);
	win->id = ft_strdup(id);
}

/*
 * This function should probably be called right after loading the ui,
 * otherwise you might save the ui_window to a variable that is used
 * somewhere else, or renderer...
 *
 * NOTE:
 * 	1. we will sdl_win associated renderer if it has one, otherwise make one;
*/
void	ui_window_replace_win(t_ui_window *ui_win, SDL_Window *sdl_win)
{
	t_vec4i	pos;

	ui_win->win_replaced = 1;
	if (ui_win->renderer)
		SDL_DestroyRenderer(ui_win->renderer);
	if (ui_win->win)
		SDL_DestroyWindow(ui_win->win);
	ui_win->win = sdl_win;
	SDL_GetWindowPosition(ui_win->win, &pos.x, &pos.y);
	SDL_GetWindowSize(ui_win->win, &pos.w, &pos.h);
	ui_win->pos = vec4(pos.x, pos.y, pos.w, pos.h);
	ui_win->renderer = SDL_GetRenderer(ui_win->win);
	if (!ui_win->renderer)
		ui_win->renderer
			= SDL_CreateRenderer(sdl_win, -1, SDL_RENDERER_ACCELERATED);
	else
		ui_win->renderer_replaced = 1;
	ui_win->texture = ui_create_texture(ui_win->renderer,
			vec2i(ui_win->pos.w, ui_win->pos.h));
	ui_win->window_id = SDL_GetWindowID(ui_win->win);
	ui_win->screen_pos.w = pos.w;
	ui_win->screen_pos.h = pos.h;
	ui_win->texture_scale = vec2(ui_win->screen_pos.w / ui_win->pos.w,
			ui_win->screen_pos.h / ui_win->pos.h);
	ui_win->textures_recreate = 1;
}

void	ui_window_print(t_ui_window *win)
{
	ft_printf("[%s]\n", __FUNCTION__);
	ft_printf("\tid : %s\n", win->id);
	ft_printf("\tpos : ");
	print_vec(win->pos.v, 4);
	ft_printf("\tscreen_pos : ");
	print_veci(win->screen_pos.v, 4);
	ft_printf("\ttexture_scale : ");
	print_vec(win->texture_scale.v, 2);
}

void	ui_window_render_texture(t_ui_window *win, SDL_Texture *texture)
{
	SDL_SetRenderTarget(win->renderer, win->texture);
	SDL_RenderCopy(win->renderer, texture, NULL, NULL);
	SDL_SetRenderTarget(win->renderer, NULL);
}
