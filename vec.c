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

void	print_vec(float *vec, size_t size)
{
	size_t	i;

	i = -1;
	ft_printf("vec { ");
	while (++i < size)
		ft_printf("%.2f ", vec[i]);
	ft_printf("}\n");
}
