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
	elem->pos.x = 0;
	elem->pos.y = 0;
	elem->pos.w = 50;
	elem->pos.h = 20;
	elem->state = UI_STATE_DEFAULT;
	elem->colors[UI_STATE_DEFAULT] = 0xff95D7AE;
	elem->colors[UI_STATE_HOVER] = 0xff7BAE7F;
	elem->colors[UI_STATE_CLICK] = 0xff73956F;
	elem->images[UI_STATE_DEFAULT] = NULL;
	elem->images[UI_STATE_HOVER] = NULL;
	elem->images[UI_STATE_CLICK] = NULL;
	elem->use_images = 0;
	elem->texture_recreate = 1;
	ui_element_textures_redo(elem);
	elem->parent = win;
	elem->parent_type = UI_TYPE_WINDOW;
	elem->parent_show = &win->show;
	elem->element = NULL;
	elem->element_type = UI_TYPE_NONE;
	elem->show = 1;
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
	elem->texture_recreate = 0;
	if (elem->use_images)
	{
		i = -1;
		while (++i < UI_STATE_AMOUNT)
		{
			if (!elem->images[i])
				continue ;
			SDL_SetRenderTarget(elem->win->renderer, elem->textures[i]);
			SDL_RenderCopy(elem->win->renderer, elem->images[i], NULL, NULL);
		}
		return ;
	}
	ui_texture_fill(elem->win->renderer, elem->textures[UI_STATE_DEFAULT], elem->colors[UI_STATE_DEFAULT]);
	ui_texture_fill(elem->win->renderer, elem->textures[UI_STATE_HOVER], elem->colors[UI_STATE_HOVER]);
	ui_texture_fill(elem->win->renderer, elem->textures[UI_STATE_CLICK], elem->colors[UI_STATE_CLICK]);
}

/*
 * Return: 1 if rendering successful, 0 if not.
*/
int	ui_element_render(t_ui_element *elem)
{
	t_vec4i parent_pos;

	if (elem->parent_type == UI_TYPE_WINDOW)
		parent_pos = ((t_ui_window *)elem->parent)->screen_pos;
	else
		parent_pos = ((t_ui_element *)elem->parent)->screen_pos;
	if (!*elem->parent_show || !elem->show)
		return (0);
	elem->screen_pos = elem->pos;
	elem->screen_pos.x = parent_pos.x + elem->pos.x;
	elem->screen_pos.y = parent_pos.y + elem->pos.y;

	if (elem->texture_recreate)
		ui_element_textures_redo(elem);

	SDL_SetRenderTarget(elem->win->renderer, NULL);
	SDL_RenderCopy(elem->win->renderer, elem->textures[elem->state], NULL,
		&(SDL_Rect){elem->screen_pos.x, elem->screen_pos.y, elem->pos.w, elem->pos.h});
	return (1);
}

/*
 * Editing functions after this.
*/
/*
 * When changing position we need to recreate the textures,
 * if the user has decided to change the w and/or h.
*/
void	ui_element_pos_set(t_ui_element *elem, t_vec4i pos)
{
	if (elem->pos.w != pos.w || elem->pos.h != pos.h)
		elem->texture_recreate = 1;
	elem->pos = pos;
}

void	ui_element_color_set(t_ui_element *elem, int state, Uint32 color)
{
	if (state < 0 || state > UI_STATE_AMOUNT)
		return ;
	if (elem->colors[state] != color)
		elem->texture_recreate = 1;
	elem->colors[state] = color;
}

void	ui_element_image_set_from_path(t_ui_element *elem, int state, char *image_path)
{
	SDL_Surface	*image;

	image = IMG_Load(image_path);
	if (!image)
		return ;
	ui_element_image_set(elem, state, image);
	SDL_FreeSurface(image);
}

void	ui_element_image_set(t_ui_element *elem, int state, SDL_Surface *image)
{
	if (state < 0 || state > UI_STATE_AMOUNT)
		return ;
	if (elem->images[state])
		SDL_DestroyTexture(elem->images[state]);
	elem->images[state] = SDL_CreateTextureFromSurface(elem->win->renderer, image);
	elem->use_images = 1;
}

void	ui_element_parent_set(t_ui_element *elem, t_ui_element *parent, int type, bool *show)
{
	elem->parent = parent;
	elem->parent_type = type;
	elem->parent_show = show;
}
