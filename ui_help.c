#include "libui_v3.h"

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

/*
 * Returns text starting from char[0] till the char at pos x (coordinate on the screen)
 * NOTE: remember to remove the offset of the text, this only takes into consideration the full str,
 * starting at x 0 for the first char.
*/
char	*get_text_to_char_at_x(char *str, int x, TTF_Font *font)
{
	char	*result;
	int		char_w;
	int		total_w;
	int		i;
	char	temp[2];

	total_w = 0;
	i = -1;
	temp[1] = '\0';
	while (str[++i] && x < total_w)
	{
		temp[0] = str[i];
		TTF_SizeText(font, temp, &char_w, NULL);
		total_w += char_w;
	}
	result = ft_strsub(str, 0, i);
	return (result);
}

/*
 * Returns x of the "nth_char" position in the str.
 * NOTE: we assume that char[0] is at 0 x.
*/
int	get_x_of_char_in_text(char *str, int nth_char, TTF_Font *font)
{
	int		result;
	char	*temp;

	temp = ft_strndup(str, nth_char);
	TTF_SizeText(font, temp, &result, NULL);
	ft_strdel(&temp);
	return (result);
}

/*
 * Returns how many:eth char is at position x in the str.
 * NOTE: assuming str[0] is at x : 0 .
*/
int	get_nth_char_of_text_at_x(char *str, int x, TTF_Font *font)
{
	int		char_w;
	int		total_w;
	int		i;
	char	temp[2];

	total_w = 0;
	i = -1;
	temp[1] = '\0';
	while (str[++i] && total_w < x)
	{
		temp[0] = str[i];
		TTF_SizeText(font, temp, &char_w, NULL);
		total_w += char_w;
	}
	return (i);
}
