#include "libui_v3.h"

void	print_vec(float *vec, size_t size)
{
	size_t	i;

	i = -1;
	ft_printf("vec { ");
	while (++i < size)
		ft_printf("%.2f ", vec[i]);
	ft_printf("}\n");
}
