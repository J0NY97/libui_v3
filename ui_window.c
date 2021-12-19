/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_window.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 19:24:56 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/12 17:17:43 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	win->texture = ui_create_texture(win->renderer,
			vec2i(win->pos.w, win->pos.h));
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
	ft_strdel(&win->title);
	ui_list_element_free(&win->children);
	SDL_DestroyTexture(win->texture);
	if (!win->renderer_replaced)
		SDL_DestroyRenderer(win->renderer);
	if (!win->win_replaced)
		SDL_DestroyWindow(win->win);
	win->layout = NULL;
	if (win->free_me)
		free(win);
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

void	ui_window_render_texture(t_ui_window *win, SDL_Texture *texture)
{
	SDL_SetRenderTarget(win->renderer, win->texture);
	SDL_RenderCopy(win->renderer, texture, NULL, NULL);
	SDL_SetRenderTarget(win->renderer, NULL);
}
