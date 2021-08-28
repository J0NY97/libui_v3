#include "libui_v3.h"

void	ui_window_new(t_ui_window *win, char *title, t_vec4 pos)
{
	memset(win, 0, sizeof(t_ui_window));
	if (!title)
		win->title = ft_strdup("Window Title Not Set");
	else
		win->title = ft_strdup(title);
	win->pos = pos;
	win->win = SDL_CreateWindow(win->title, win->pos.x, win->pos.y, win->pos.w, win->pos.h, 0);
	win->renderer = SDL_CreateRenderer(win->win, -1, SDL_RENDERER_ACCELERATED);
	win->texture = SDL_CreateTexture(win->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, win->pos.w, win->pos.h);
	win->show = 1;

	win->window_id = SDL_GetWindowID(win->win);

	SDL_GetWindowSize(win->win, &win->screen_pos.w, &win->screen_pos.h);
	win->texture_scale = vec2(win->pos.w / win->screen_pos.w, win->pos.h / win->screen_pos.h);
	SDL_GetMouseState(&win->window_mouse_pos.x, &win->window_mouse_pos.y);
	win->mouse_pos.x = win->window_mouse_pos.x * win->texture_scale.x;
	win->mouse_pos.y = win->window_mouse_pos.y * win->texture_scale.y; 
}

void	ui_window_event(t_ui_window *win, SDL_Event e)
{
	if (win->window_id == e.window.windowID)
	{
		if (e.type == SDL_WINDOWEVENT)
		{
			if (e.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				SDL_GetWindowSize(win->win, &win->screen_pos.w, &win->screen_pos.h);
				win->texture_scale = vec2(win->pos.w / win->screen_pos.w, win->pos.h / win->screen_pos.h);
			}
		}
		else if (e.type == SDL_MOUSEMOTION)
		{
			SDL_GetMouseState(&win->window_mouse_pos.x, &win->window_mouse_pos.y);
			win->mouse_pos.x = win->window_mouse_pos.x * win->texture_scale.x;
			win->mouse_pos.y = win->window_mouse_pos.y * win->texture_scale.y;
		}
	}
	if (e.button.type == SDL_MOUSEBUTTONDOWN)
		win->mouse_down = 1;
	else if (e.button.type == SDL_MOUSEBUTTONUP)
		win->mouse_down = 0;
}

int	ui_window_render(t_ui_window *win)
{
	SDL_SetRenderTarget(win->renderer, NULL);
	SDL_RenderCopy(win->renderer, win->texture, NULL, NULL);

	SDL_SetRenderTarget(win->renderer, win->texture);
	SDL_RenderClear(win->renderer);
	SDL_SetRenderTarget(win->renderer, NULL);
	return (1);
}

void	ui_window_free(void *win)
{
	(void)win;
}

/*
 * Window edititing functions down from here.
*/

void	ui_window_flag_set(t_ui_window *win, int flags)
{
	if (flags & UI_WINDOW_HIDE)
		SDL_HideWindow(win->win);
	if (flags & UI_WINDOW_SHOW)
		SDL_ShowWindow(win->win);
	if (flags & UI_WINDOW_MAXIMIZE)
		SDL_MaximizeWindow(win->win);
	if (flags & UI_WINDOW_MINIMIZE)
		SDL_MinimizeWindow(win->win);
	if (flags & UI_WINDOW_FULLSCREEN)
		SDL_SetWindowFullscreen(win->win, SDL_WINDOW_FULLSCREEN);
	if (flags & UI_WINDOW_FULLSCREEN_WINDOWED)
		SDL_SetWindowFullscreen(win->win, SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (flags & UI_WINDOW_GRAB)
		SDL_SetWindowGrab(win->win, SDL_TRUE);
	if (flags & UI_WINDOW_RESIZEABLE)
		SDL_SetWindowResizable(win->win, SDL_TRUE);
}
