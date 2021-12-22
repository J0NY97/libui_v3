/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_font.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/22 14:06:15 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/22 14:50:02 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"

typedef struct	s_ui_font
{
	char		*path;
	int			size;
	TTF_Font	*font;
}				t_ui_font;

/*
 * Returns null if path doesnt exits;
*/
TTF_Font	*ui_get_font(char *path, int size)
{
	static t_ui_font	*fonts = NULL;
	static size_t		font_count = 0;
	int					i;

	if (access(path, F_OK))
		return (NULL);
	i = -1;
	while (++i < font_count)
		if (fonts[i].size == size)
			if (ft_strequ(fonts[i].path, path))
				return (fonts[i].font);
	fonts = realloc(fonts, ++font_count * sizeof(t_ui_font));
	fonts[font_count - 1].path = ft_strdup(path);
	fonts[font_count - 1].size = size;
	fonts[font_count - 1].font = TTF_OpenFont(path, size);
	ft_printf("[%s] Font count : %d\n", __FUNCTION__, font_count);
	return (fonts[font_count - 1].font);
}
