#include "libui_v3.h"

/*
 * All elements should be in stack memory,
 * and you should pass in the address of it to this function,
 * which then reset the values to the default values.
 *
 * NOTE: if you resize the element, you have to update the texture.
*/
void	ui_element_new(t_ui_window *win, t_ui_element *elem)
{
	memset(elem, 0, sizeof(t_ui_element));
	elem->win = win;
	elem->pos.x = 10;
	elem->pos.y = 10;
	elem->pos.w = 50;
	elem->pos.h = 20;
	elem->state = UI_STATE_DEFAULT;
	ui_element_textures_redo(elem);
	ui_texture_fill_rect(elem->win->renderer, elem->textures[UI_STATE_DEFAULT], 0xff95D7AE);
	elem->parent = win;
	elem->parent_type = UI_TYPE_WINDOW;
	elem->element = NULL;
	elem->element_type = UI_TYPE_NONE;
}

/*
 * Redos the textures, helpful if you have changed the w and/or h of the element.
*/
void	ui_element_textures_redo(t_ui_element *elem)
{
	int	i;

	i = -1;
	while (++i < UI_STATE_AMOUNT)
	{
		if (elem->textures[i])
			SDL_DestroyTexture(elem->textures[i]);
	}
	elem->textures[UI_STATE_DEFAULT] = ui_create_texture(elem->win->renderer, elem->pos);
	elem->textures[UI_STATE_HOVER] = ui_create_texture(elem->win->renderer, elem->pos);
	elem->textures[UI_STATE_CLICK] = ui_create_texture(elem->win->renderer, elem->pos);
}

/*
 * When changing position we need to recreate the textures,
 * if the user has decided to change the w and/or h.
*/
void	ui_element_pos_set(t_ui_element *elem, t_vec4i pos)
{
	int	redo;

	redo = 0;
	if (elem->pos.w != pos.w || elem->pos.h != pos.h)
		redo = 1;
	elem->pos = pos;
	if (redo)
		ui_element_textures_redo(elem);
}

void	ui_element_render(t_ui_element *elem)
{
	t_vec4i parent_pos;

	if (elem->parent_type == UI_TYPE_WINDOW)
		parent_pos = ((t_ui_window *)elem->parent)->screen_pos;
	else
		parent_pos = ((t_ui_element *)elem->parent)->screen_pos;
	elem->screen_pos = elem->pos;
	elem->screen_pos.x = parent_pos.x + elem->pos.x;
	elem->screen_pos.y = parent_pos.y + elem->pos.y;

	SDL_SetRenderTarget(elem->win->renderer, NULL);
	SDL_RenderCopy(elem->win->renderer, elem->textures[elem->state], NULL,
		&(SDL_Rect){elem->screen_pos.x, elem->screen_pos.y, elem->pos.w, elem->pos.h});
}
