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

void	ui_surface_line_draw_dot(SDL_Surface *surface, t_vec2i p1, t_vec2i p2, Uint32 color)
{
	bool	y_longer;
	int		increment_val;
	int		end_val;
	int		short_len;
	int		long_len;
	int		dec_inc;
	int		j;
	int		i;
	int		gap;

	y_longer = false;
	short_len = p2.y - p1.y;
	long_len = p2.x - p1.x;
	j = 0;
	i = 0;
	gap = 5;
	y_longer = abs(short_len) > abs(long_len);
	if (y_longer)
		ft_swap(&short_len, &long_len);
	end_val = long_len;
	increment_val = 1;
	if (long_len < 0)
	{
		increment_val = -1;
		long_len = -long_len;
	}
	dec_inc = 0;
	if (long_len != 0)
		dec_inc = (short_len << 16) / long_len;
	if (y_longer)
	{
		while (i != end_val)
		{
			if (i % 5 != 0) // the 5 is the gap
				ui_surface_pixel_set(surface, p1.x + (j >> 16), p1.y + i, color);	
			j += dec_inc;
			i += increment_val;
		}
		return ;
	}
	while (i != end_val)
	{
		if (i % 5 != 0) // the 5 is the gap
			ui_surface_pixel_set(surface, p1.x + i, p1.y + (j >> 16), color);
		j += dec_inc;
		i += increment_val;
	}
}

/*
 * Line algorithm yoinked from:
 * http://elynxsdk.free.fr/ext-docs/Rasterization/Lines/Lines%20algorithms.htm
 * http://www.edepot.com/lined.html
*/
void	ui_surface_line_draw_orig(SDL_Surface *surface, t_vec2i p1, t_vec2i p2, Uint32 color)
{
	bool	y_longer = false;
	int		increment_val;
	int		end_val;
	int		short_len = p2.y - p1.y;
	int		long_len= p2.x - p1.x;
	int		dec_inc;
	int		swap;
	int		j;
	int		i;

	if (abs(short_len) > abs(long_len))
	{
		swap = short_len;
		short_len = long_len;
		long_len = swap;
		y_longer = true;
	}
	end_val = long_len;
	if (long_len < 0)
	{
		increment_val = -1;
		long_len = -long_len;
	}
	else
		increment_val = 1;
	if (long_len==0)
		dec_inc=0;
	else
		dec_inc = (short_len << 16) / long_len;
	j = 0;
	if (y_longer)
	{	
		i = 0;
		while (i != end_val)
		{
			ui_surface_pixel_set(surface, p1.x + (j >> 16), p1.y + i, color);	
			j += dec_inc;
			i += increment_val;
		}
	}
	else
	{
		i = 0;
		while (i != end_val)
		{
			ui_surface_pixel_set(surface, p1.x + i, p1.y + (j >> 16), color);
			j += dec_inc;
			i += increment_val;
		}
	}
}

/*
 * Reduced version.
*/
void	ui_surface_line_draw(SDL_Surface *surface, t_vec2i p1, t_vec2i p2, Uint32 color)
{
	bool	y_longer;
	int		increment_val;
	int		end_val;
	int		short_len;
	int		long_len;
	int		dec_inc;
	int		j;
	int		i;

	y_longer = false;
	short_len = p2.y - p1.y;
	long_len = p2.x - p1.x;
	j = 0;
	i = 0;
	y_longer = abs(short_len) > abs(long_len);
	if (y_longer)
		ft_swap(&short_len, &long_len);
	end_val = long_len;
	increment_val = 1;
	if (long_len < 0)
	{
		increment_val = -1;
		long_len = -long_len;
	}
	dec_inc = 0;
	if (long_len != 0)
		dec_inc = (short_len << 16) / long_len;
	if (y_longer)
	{
		while (i != end_val)
		{
			ui_surface_pixel_set(surface, p1.x + (j >> 16), p1.y + i, color);	
			j += dec_inc;
			i += increment_val;
		}
		return ;
	}
	while (i != end_val)
	{
		ui_surface_pixel_set(surface, p1.x + i, p1.y + (j >> 16), color);
		j += dec_inc;
		i += increment_val;
	}
}

// niklas version
static inline int	sign(float a, float b)
{
	if (a < b)
		return (1);
	return (-1);
}

static inline void	line_calc(t_vec2i v1, t_vec2i v2,
		int *cath_x, int *cath_y)
{
	if (((int)v2.y - (int)v1.y) < 0)
		*cath_x = ((int)v2.y - (int)v1.y) * -1;
	else
		*cath_x = ((int)v2.y - (int)v1.y);
	if (((int)v2.x - (int)v1.x) < 0)
		*cath_y = ((int)v2.x - (int)v1.x) * -1;
	else
		*cath_y = ((int)v2.x - (int)v1.x);
}

void	ui_surface_line_draw_nik(SDL_Surface *surf, t_vec2i v1, t_vec2i v2, Uint32 color)
{
	int			cath_x;
	int			cath_y;
	int			overflow_x;
	int			overflow_y;

	line_calc(v1, v2, &cath_x, &cath_y);
	overflow_y = cath_y - cath_x;
	while ((int)v1.x != (int)v2.x || (int)v1.y != (int)v2.y)
	{
		ui_surface_pixel_set(surf, (int)v1.x, (int)v1.y, color);
		overflow_x = overflow_y * 2;
		if (overflow_x > -(cath_x))
		{
			overflow_y -= cath_x;
			v1.x += sign((int)v1.x, (int)v2.x);
		}
		else if (overflow_x < cath_x)
		{
			overflow_y += cath_y;
			v1.y += sign((int)v1.y, (int)v2.y);
		}
	}
}
