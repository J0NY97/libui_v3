#include "libui_v3.h"

t_vec4i	vec4i(int x, int y, int w, int h)
{
	t_vec4i	v;

	v.x = x;
	v.y = y;
	v.w = w;
	v.h = h;
	return (v);
}

int	point_in_rect(t_vec2i point, t_vec4i rect)
{
	if (point.x >= rect.x
		&& point.y >= rect.y
		&& point.x < rect.x + rect.w
		&& point.y < rect.y + rect.h)
		return (1);
	return (0);
}

t_rgba	hex_to_rgba(Uint32 color_hex)
{
	t_rgba	rgba;

	rgba.a = (color_hex >> 24 & 0xff);
	rgba.r = (color_hex >> 16 & 0xff);
	rgba.g = (color_hex >> 8 & 0xff);
	rgba.b = (color_hex & 0xff);
	return (rgba);
}

Uint32	rgba_to_hex(t_rgba rgba)
{
	return ((rgba.a & 0xff) << 24
		| ((rgba.r & 0xff) << 16)
		| ((rgba.g & 0xff) << 8)
		| (rgba.b & 0xff));
}
