#include "libui_v3.h"

SDL_Surface	*ui_create_surface(t_vec4i size)
{
	SDL_Surface	*surface;

	surface = SDL_CreateRGBSurface(0, size.w, size.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	return (surface);
}

void	ui_surface_draw_rect(SDL_Surface *surface, t_vec4i rect, Uint32 color)
{
	Uint32	*pixels;

	pixels = surface->pixels;
	memcpy(pixels[(rect.y * surface->pitch) + (rect.x * surface->format->BytesPerPixel)], color, sizeof(Uint32) * rect.w);
	memcpy(pixels[((rect.y * surface->pitch) * rect.h) + (rect.x * surface->format->BytesPerPixel)], color, sizeof(Uint32) * rect.w);
}

void	ui_surface_draw_border(SDL_Surface *surface, size_t thicc, Uint32 color)
{
	t_vec4i	rect;
	size_t	i;

	i = -1;
	while (++i < thicc)
	{
		rect.x = i;
		rect.y = i;
		rect.w = surface->w - i - 1;
		rect.h = surface->h - i - 1;
		ui_surface_draw_rect(surface, rect, color);
	}
}

void	ui_surface_fill(SDL_Surface *surface, Uint32 color)
{
	SDL_FillRect(surface, NULL, color);
	/*
	ft_printf("surface bpp: %d\n", surface->format->BytesPerPixel);
	ft_printf("pitch : %d\n", surface->pitch);
	ft_printf("w * bpp : %d\n", surface->w * surface->format->BytesPerPixel);
	ft_printf("sizeof(Uint32) : %d\n", sizeof(Uint32));
	ft_printf("color in pixels[0] : %#x\n", ((Uint32 *)surface->pixels)[0]);
//	memcpy(surface->pixels, &color, surface->h * surface->pitch);
//	memcpy(surface->pixels, &color, surface->h * (surface->w * surface->format->BytesPerPixel));
	ft_printf("finally we filled surface\n");
	*/
}
