/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 12:03:34 by nneronin          #+#    #+#             */
/*   Updated: 2022/02/27 12:31:17 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define SDL_MAIN_HANDLED
#include "stdlib.h"
#include "libft.h"
#include "libpf.h"
#include "fcntl.h"
#include "sys/stat.h"
#include "SDL.h"

char	*build_non_rword_path(char *rword_path)
{
	int		i;

	i = 0;
	while (rword_path[i])
	{
		if (rword_path[i] == '\\')
			rword_path[i] = '/';
		i++;
	}
	return (rword_path);
}

int	main(void)
{
	int		fd;
	char	*line;
	char	*path;
	char	*non_rword_path;

	fd = creat("ui_path.h", S_IRUSR | S_IWUSR);
	path = SDL_GetBasePath();
	build_non_rword_path(path);
	ft_dprintf(fd,
		"#ifndef UI_PATH_H\n"\
		"# define UI_PATH_H\n"\
		"# define UI_PATH \"%s\"\n"\
		"# define UI_FONT_PATH \"%sfonts/\"\n"\
		"# define UI_IMAGE_PATH \"%simages/\"\n"\
		"#endif"\
		, path, path, path);
	close(fd);
	SDL_free(path);
	return (1);
}
