#include "libui.h"

void	ui_window_title_set(t_ui_window *win, const char *title)
{
	if (win->title)
		ft_strdel(&win->title);
	win->title = ft_strdup(title);
	SDL_SetWindowTitle(win->win, title);
}

void	ui_window_id_set(t_ui_window *win, const char *id)
{
	if (!id)
		return ;
	if (win->id)
		ft_strdel(&win->id);
	win->id = ft_strdup(id);
}

void	ui_window_print(t_ui_window *win)
{
	ft_printf("[%s]\n", __FUNCTION__);
	ft_printf("\tid : %s\n", win->id);
	ft_printf("\tpos : ");
	print_vec(win->pos.v, 4);
	ft_printf("\tscreen_pos : ");
	print_veci(win->screen_pos.v, 4);
	ft_printf("\ttexture_scale : ");
	print_vec(win->texture_scale.v, 2);
}
