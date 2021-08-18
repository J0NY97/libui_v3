#include "libui_v3.h"

void	ui_window_new(t_ui_window *win, char *title, t_vec4i pos)
{
	memset(win, 0, sizeof(t_ui_window));
	win->title = ft_strdup(title);
	win->pos = pos;
	win->win = SDL_CreateWindow(win->title, win->pos.x, win->pos.y, win->pos.w, win->pos.h, 0);
	win->renderer = SDL_CreateRenderer(win->win, -1, SDL_RENDERER_ACCELERATED);
	win->texture = SDL_CreateTexture(win->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, win->pos.w, win->pos.h);
	win->show = 1;
}

void	ui_window_event(t_ui_window *win, SDL_Event e)
{
	SDL_GetMouseState(&win->mouse_pos.x, &win->mouse_pos.y);
	if (e.button.type == SDL_MOUSEBUTTONDOWN)
		win->mouse_down = 1;
	else if (e.button.type == SDL_MOUSEBUTTONUP)
		win->mouse_down = 0;
}

void	ui_window_render(t_ui_window *win)
{
	SDL_SetRenderTarget(win->renderer, NULL);
	SDL_RenderCopy(win->renderer, win->texture, NULL, NULL);
}

void	ui_window_free(void *win)
{
	(void)win;
}
