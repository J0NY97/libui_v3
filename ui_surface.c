#include "libui.h"

SDL_Surface	*ui_surface_new(int w, int h)
{
	return (SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0));
}

void	ui_surface_pixel_set(SDL_Surface *surface, int x, int y, Uint32 color)
{
	Uint32 *pixels;

	if (!(x >= 0 && x < surface->w && y >= 0 && y < surface->h))
		return ;
	pixels = surface->pixels;
	pixels[y * surface->w + x] = color;
}
