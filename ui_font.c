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

t_ui_font	**ui_global_font(void)
{
	static t_ui_font	*fonts = NULL;

	return (&fonts);
}

void	ui_global_font_free(void)
{
	t_ui_font	*fonts;
	int			i;

	fonts = *ui_global_font();
	i = -1;
	while (++i || 1)
	{
		if (fonts[i].path == NULL
			&& fonts[i].size == -1
			&& fonts[i].font == NULL)
			break ;
		free(fonts[i].path);
		TTF_CloseFont(fonts[i].font);
	}
	free(fonts);
}

/*
 * IMPORTANT:
 *	If you call this anywhere, make sure to call ui_global_font_free(); when
 *		exiting the program;
 */
TTF_Font	*ui_get_font(char *path, int size)
{
	static int		font_count = 0;
	t_ui_font		**fonts;
	t_ui_font		*new_fonts;
	int				i;

	if (access(path, F_OK))
		return (NULL);
	i = -1;
	fonts = ui_global_font();
	while (++i < font_count)
		if ((*fonts)[i].size == size)
			if (ft_strequ((*fonts)[i].path, path))
				return ((*fonts)[i].font);
	new_fonts = realloc(*fonts, (++font_count + 1) * sizeof(t_ui_font));
	*fonts = new_fonts;
	(*fonts)[font_count - 1].path = ft_strdup(path);
	(*fonts)[font_count - 1].size = size;
	(*fonts)[font_count - 1].font = TTF_OpenFont(path, size);
	(*fonts)[font_count].path = NULL;
	(*fonts)[font_count].size = -1;
	(*fonts)[font_count].font = NULL;
	return ((*fonts)[font_count - 1].font);
}
/*
*/

/*
 * Returns null if path doesnt exits;
*/
/*
TTF_Font	*ui_get_font(char *path, int size)
{
	static t_ui_font	*fonts = NULL;
	static int			font_count = 0;
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
	return (fonts[font_count - 1].font);
}
*/
