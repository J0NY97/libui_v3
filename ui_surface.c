#include "libui.h"

SDL_Surface	*ui_surface_new(int w, int h)
{
	return (SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0));
}

// TODO: change this name to pixel_draw?
void	ui_surface_pixel_set(SDL_Surface *surface, int x, int y, Uint32 color)
{
	Uint32 *pixels;

	if (!(x >= 0 && x < surface->w && y >= 0 && y < surface->h))
		return ;
	pixels = surface->pixels;
	pixels[y * surface->w + x] = color;
}

void	ui_surface_line_draw(SDL_Surface *surface, t_vec2i p1, t_vec2i p2, Uint32 color)
{
	bool	yLonger = false;
	int		incrementVal;
	int		endVal;
	int		shortLen = p2.y - p1.y;
	int		longLen= p2.x - p1.x;
	int		decInc;
	int		swap;
	int		j;

	if (abs(shortLen) > abs(longLen))
	{
		swap = shortLen;
		shortLen = longLen;
		longLen = swap;
		yLonger = true;
	}
	endVal = longLen;
	if (longLen < 0)
	{
		incrementVal = -1;
		longLen = -longLen;
	}
	else
		incrementVal = 1;
	if (longLen==0)
		decInc=0;
	else
		decInc = (shortLen << 16) / longLen;
	j = 0;
	if (yLonger)
	{	
		for (int i = 0; i != endVal; i += incrementVal)
		{
			ui_surface_pixel_set(surface, p1.x + (j >> 16), p1.y + i, color);	
			j += decInc;
		}
	}
	else
	{
		for (int i = 0; i != endVal; i += incrementVal)
		{
			ui_surface_pixel_set(surface, p1.x + i, p1.y + (j >> 16), color);
			j += decInc;
		}
	}
}
