#include "libui.h"

SDL_Texture	*ui_create_texture(SDL_Renderer *renderer, t_vec2i pos)
{
	SDL_Texture	*texture;

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_TARGET, pos.x, pos.y);
	return (texture);
}

void	ui_texture_print(SDL_Texture *texture)
{
	Uint32	format;
	int		access;
	int		w;
	int		h;

	if (!texture)
	{
		ft_printf("[%s] Texture given doesn\'t exist.\n", __FUNCTION__);
		return ;
	}
	ft_printf("[%s]\n", __FUNCTION__);
	SDL_QueryTexture(texture, &format, &access, &w, &h);
	ft_printf("\tw : %d\n", w);
	ft_printf("\th : %d\n", h);
	ft_printf("\tformat : %s\n", SDL_GetPixelFormatName(format));
}

void	ui_texture_fill(
		SDL_Renderer *renderer, SDL_Texture *texture, Uint32 color)
{
	t_rgba		rgba;

	rgba = hex_to_rgba(color);
	SDL_SetRenderTarget(renderer, texture);
	SDL_SetRenderDrawColor(renderer, rgba.r, rgba.g, rgba.b, rgba.a);
	SDL_RenderFillRect(renderer, NULL);
	SDL_SetRenderTarget(renderer, NULL);
}

void	ui_texture_fill_rect(
		SDL_Renderer *renderer, SDL_Texture *texture,
		Uint32 color, t_vec4i rect)
{
	t_rgba		rgba;

	rgba = hex_to_rgba(color);
	SDL_SetRenderTarget(renderer, texture);
	SDL_SetRenderDrawColor(renderer, rgba.r, rgba.g, rgba.b, rgba.a);
	SDL_RenderFillRect(renderer, &(SDL_Rect){rect.x, rect.y, rect.w, rect.h});
	SDL_SetRenderTarget(renderer, NULL);
}

void	ui_texture_draw_border(
		SDL_Renderer *renderer, SDL_Texture *texture,
		size_t thicc, Uint32 color)
{
	t_rgba		rgba;
	SDL_Rect	rect;
	size_t		i;
	int			w;
	int			h;

	i = -1;
	rgba = hex_to_rgba(color);
	SDL_SetRenderTarget(renderer, texture);
	SDL_SetRenderDrawColor(renderer, rgba.r, rgba.g, rgba.b, rgba.a);
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	while (++i < thicc)
	{
		rect.x = i;
		rect.y = i;
		rect.w = w - i - 1;
		rect.h = h - i - 1;
		SDL_RenderDrawRect(renderer, &rect);
	}
	SDL_SetRenderTarget(renderer, NULL);
}

/*
 * 1. First try to find with absolute path...
 * 2. ... if not found, try to find from UI_FONT_PATH/"font_name" ...
 * 3. ... if still not found, try to find default font.
 * 		IF this is not found, it's my fault.
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
