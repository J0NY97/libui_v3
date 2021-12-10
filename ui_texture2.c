/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_texture2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 19:24:41 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/10 19:24:41 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"

/*
 * 1. First try to find with absolute path...
 * 2. ... if not found, try to find from UI_FONT_PATH/"font_name" ...
 * 3. ... if still not found, try to find default font.
 * 		IF this is not found, it's my fault.
 *
 * Seg faults if font_path == NULL;
*/
char	*get_font_path(char *font_path)
{
	char	*temp_font_path;

	temp_font_path = ft_strdup(font_path);
	if (access(temp_font_path, F_OK))
	{
		ft_strdel(&temp_font_path);
		temp_font_path = ft_strjoin(UI_FONT_PATH, font_path);
	}
	else
		return (temp_font_path);
	if (access(temp_font_path, F_OK))
	{
		ft_strdel(&temp_font_path);
		temp_font_path = ft_strdup(UI_FONT_PATH"DroidSans.ttf");
		if (!access(temp_font_path, F_OK))
			return (temp_font_path);
		else
		{
			ft_strdel(&temp_font_path);
			return (NULL);
		}
	}
	return (temp_font_path);
}

SDL_Surface	*ui_surface_create_from_text_recipe(t_ui_label *recipe)
{
	SDL_Surface	*surface;
	t_rgba		rgba;
	SDL_Color	color;
	char		*temp;

	if (!recipe->font || recipe->font_recreate)
	{
		temp = get_font_path(recipe->font_path);
		ft_strdel(&recipe->font_path);
		recipe->font_path = temp;
		if (recipe->font)
			TTF_CloseFont(recipe->font);
		recipe->font = TTF_OpenFont(recipe->font_path, recipe->font_size);
		recipe->font_recreate = 0;
	}
	TTF_SizeUTF8(recipe->font, recipe->text,
		&recipe->text_wh.x, &recipe->text_wh.y);
	color = rgba_to_sdl_color(hex_to_rgba(recipe->font_color));
	if (recipe->max_w == -1)
		surface = TTF_RenderUTF8_Blended(recipe->font, recipe->text, color);
	else
		surface = TTF_RenderUTF8_Blended_Wrapped(recipe->font,
				recipe->text, color, recipe->max_w);
	return (surface);
}

SDL_Texture	*ui_texture_create_from_text_recipe(
		SDL_Renderer *renderer, t_ui_label *recipe)
{
	SDL_Texture	*texture;
	SDL_Surface	*surface;

	surface = ui_surface_create_from_text_recipe(recipe);
	if (!surface)
		return (NULL);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return (texture);
}

SDL_Texture	*ui_texture_create_from_path(SDL_Renderer *renderer, char *path)
{
	SDL_Surface	*surface;
	SDL_Texture	*texture;

	surface = ui_surface_image_new(path);
	if (!surface)
	{
		ft_printf("[%s] Surface couldn\'t be created from path. <%s>\n",
			__FUNCTION__, path);
		return (NULL);
	}
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture)
		ft_printf("[%s] Texture couldn\'t be created from surface. <%s>\n",
			__FUNCTION__, path);
	SDL_FreeSurface(surface);
	return (texture);
}
