#include "libui.h"

int	point_in_rect(t_vec2i point, t_vec4i rect)
{
	if (point.x >= rect.x
		&& point.y >= rect.y
		&& point.x < rect.x + rect.w
		&& point.y < rect.y + rect.h)
		return (1);
	return (0);
}

int	rect_in_rect(t_vec4i rect1, t_vec4i rect2)
{
	if (rect1.x < rect2.x + rect2.w
		&& rect1.x + rect1.w > rect2.x
		&& rect1.y < rect2.y + rect2.h
		&& rect1.h + rect1.y > rect2.y)
		return (1);
	return (0);
}

void	pointer_swap(void *p1, void *p2)
{
	void	*temp;

	temp = p1;
	p1 = p2;
	p2 = temp;
}

SDL_Rect	create_sdl_rect(int x, int y, int w, int h)
{
	return ((SDL_Rect){x, y, w, h});
}

SDL_Color	rgba_to_sdl_color(t_rgba rgba)
{
	SDL_Color	color;

	color.r = rgba.r;
	color.g = rgba.g;
	color.b = rgba.b;
	color.a = rgba.a;
	return (color);
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
 * Returns darkened version of the original color by procentage;
*/
Uint32	ui_color_change_brightness(Uint32 orig_col, float procentage)
{
	t_rgba	rgba;

	rgba = hex_to_rgba(orig_col);
	if (procentage < 0)
	{
		rgba.r = (255 - rgba.r) * procentage + rgba.r;
		rgba.g = (255 - rgba.g) * procentage + rgba.g;
		rgba.b = (255 - rgba.b) * procentage + rgba.b;
		return (rgba_to_hex(rgba));
	}
	procentage += 1;
	rgba.r *= procentage;
	rgba.g *= procentage;
	rgba.b *= procentage;
	return (rgba_to_hex(rgba));
}

/*
 * Returns text starting from char[0] till
 * 	the char at pos x (coordinate on the screen)
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

int	dist(t_vec2i p1, t_vec2i p2)
{
	return (sqrt((p2.x - p1.x) * (p2.x - p1.x)
			+ (p2.y - p1.y) * (p2.y - p1.y)));
}

float	fdist(t_vec2 p1, t_vec2 p2)
{
	return (sqrt((p2.x - p1.x) * (p2.x - p1.x)
			+ (p2.y - p1.y) * (p2.y - p1.y)));
}

/*
 * Every word should have whitespace between them;
*/
int	actual_word_count(char *str)
{
	int		count;
	int		i;

	if (!str)
		return (0);
	i = -1;
	count = 1;
	while (str[++i])
	{
		if (ft_isspace(str[i]))
		{
			count++;
			while (ft_isspace(str[++i]))
				;
		}
	}
	return (count);
}

char	**ft_strsplitwhitespace(char *str)
{
	char	**arr;
	int		i;
	int		prev_i;
	int		wc;

	if (!str)
		return (NULL);
	arr = malloc(sizeof(char *) * (actual_word_count(str) + 1));
	prev_i = 0;
	i = -1;
	wc = -1;
	while (str[++i])
	{
		if (ft_isspace(str[i]))
		{
			arr[++wc] = ft_strsub(str, prev_i, i - prev_i);
			while (ft_isspace(str[++i]))
				;
			prev_i = i;
		}
	}
	if (i - prev_i > 0)
		arr[++wc] = ft_strsub(str, prev_i, i - prev_i);
	arr[++wc] = NULL;
	return (arr);
}

char	*create_final(char **arr)
{
	char	*final;
	char	*trim;
	int		i;

	final = NULL;
	i = -1;
	while (arr[++i])
	{
		trim = ft_strtrim(arr[i]);
		if (!trim)
			continue ;
		ft_stradd(&final, trim);
		if (arr[i + 1])
			ft_straddchar(&final, ' ');
		ft_strdel(&trim);
	}
	ft_arraydel(arr);
	return (final);
}

/*
 * Remove all whitespace at the ends,
 * replace all whitespace with space,
 * remove comments.
*/
char	*ft_supertrim(char *str)
{
	char	*trim;
	char	**arr;

	if (!str)
		return (NULL);
	trim = ft_strtrim(str);
	if (!trim)
		return (NULL);
	if (trim[0] && trim[0] == '/' && trim[1] && trim[1] == '/')
	{
		ft_strdel(&trim);
		return (NULL);
	}
	arr = ft_strsplitwhitespace(trim);
	ft_strdel(&trim);
	return (create_final(arr));
}

char	**ft_strsplitfirstoccurence(char *str, char c)
{
	char	**arr;
	int		i;

	if (!str)
		return (NULL);
	arr = ft_memalloc(sizeof(char *) * 3);
	i = -1;
	while (str[++i] && str[i] != c)
		;
	arr[0] = ft_strsub(str, 0, i);
	if (str[i] == c && str[i + 1])
		arr[1] = ft_strsub(str, i + 1, ft_strlen(str) - 1);
	return (arr);
}

/*
 * splits at first occurence of either delim 'b' or delim 'c';
*/
char	**ft_strsplitfirstoccurenceor(char *str, char b, char c)
{
	char	**arr;
	int		i;

	if (!str)
		return (NULL);
	arr = ft_memalloc(sizeof(char *) * 3);
	i = -1;
	while (str[++i] && str[i] != c && str[i] != b)
		;
	arr[0] = ft_strsub(str, 0, i);
	if (str[i] == c || str[i] == b)
		arr[1] = ft_strsub(str, i + 1, ft_strlen(str) - 1);
	return (arr);
}

void	ft_strtrimwholearr(char **arr)
{
	int		i;
	char	*temp;

	i = -1;
	while (arr[++i])
	{
		temp = ft_strtrim(arr[i]);
		ft_strdel(&arr[i]);
		arr[i] = temp;
	}
}

char	**ft_arrdup(char **src)
{
	char	**dest;
	int		ii;

	if (!src)
		return (NULL);
	ii = -1;
	while (src[++ii])
		;
	dest = malloc(sizeof(char *) * ii);
	ii = -1;
	while (src[++ii])
		dest[ii] = ft_strdup(src[ii]);
	dest[ii] = NULL;
	return (dest);
}

int	ft_strinarr(char *str, char **arr)
{
	int	ii;

	if (!str || !arr)
		return (0);
	ii = -1;
	while (arr[++ii])
		if (ft_strequ(str, arr[ii]))
			return (1);
	return (0);
}

int	ft_arrlen(char **arr)
{
	int	i;

	if (!arr)
		return (0);
	i = -1;
	while (arr[++i])
		;
	return (i);
}

/*
 * returns arr;
*/
char	**ft_arradd(char **arr, char *add)
{
	int	len;

	if (!add)
		return (arr);
	len = ft_arrlen(arr);
	arr = realloc(arr, sizeof(char *) * (len + 2));
	arr[len] = ft_strdup(add);
	arr[len + 1] = NULL;
	return (arr);
}
