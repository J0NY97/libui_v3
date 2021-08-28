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
	elem->pos.w = 60;
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
	elem->textures[UI_STATE_DEFAULT] = ui_create_texture(elem->win->renderer, vec2i(elem->pos.w, elem->pos.h));
	elem->textures[UI_STATE_HOVER] = ui_create_texture(elem->win->renderer, vec2i(elem->pos.w, elem->pos.h));
	elem->textures[UI_STATE_CLICK] = ui_create_texture(elem->win->renderer, vec2i(elem->pos.w, elem->pos.h));
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
	t_vec4 new_pos;

	if (!*elem->parent_show || !elem->show)
		return (0);

	if (elem->parent_type == UI_TYPE_WINDOW)
		parent_pos = ((t_ui_window *)elem->parent)->screen_pos;
	else
		parent_pos = ((t_ui_element *)elem->parent)->screen_pos;

	new_pos.x = elem->pos.x;
	new_pos.y = elem->pos.y;
	new_pos.w = elem->pos.w;
	new_pos.h = elem->pos.h;
	if (elem->pos.x < 1.0f && elem->pos.x > 0.0f)
		new_pos.x = parent_pos.w * elem->pos.x;
	if (elem->pos.y < 1.0f && elem->pos.y > 0.0f)
		new_pos.y = parent_pos.h * elem->pos.y;
	if (elem->pos.w < 1.0f && elem->pos.w > 0.0f)
		new_pos.w = parent_pos.w * elem->pos.w;
	if (elem->pos.h < 1.0f && elem->pos.h > 0.0f)
		new_pos.h = parent_pos.h * elem->pos.h;

	elem->screen_pos.w = new_pos.w;
	elem->screen_pos.h = new_pos.h;
	elem->screen_pos.x = parent_pos.x + new_pos.x;
	elem->screen_pos.y = parent_pos.y + new_pos.y;

	if (elem->texture_recreate || elem->win->textures_recreate)
		ui_element_textures_redo(elem);

	SDL_SetRenderTarget(elem->win->renderer, elem->win->texture);
	SDL_RenderCopy(elem->win->renderer, elem->textures[elem->state], NULL,
		&(SDL_Rect){elem->screen_pos.x, elem->screen_pos.y, elem->screen_pos.w, elem->screen_pos.h});
	return (1);
}

/*
 * Editing functions after this.
*/
void	ui_element_pos_set(t_ui_element *elem, t_vec4 pos)
{
	if (elem->pos.w != pos.w || elem->pos.h != pos.h)
		elem->texture_recreate = 1;
	elem->pos = pos;
}

void	ui_element_pos_set2(t_ui_element *elem, t_vec2 pos)
{
	elem->pos.x = pos.x;
	elem->pos.y = pos.y;
}

void	ui_element_color_set(t_ui_element *elem, int state, Uint32 color)
{
	int	j;

	if (state < 0 || state > UI_STATE_AMOUNT)
		return ;
	if (state == UI_STATE_AMOUNT)
	{
		j = -1;
		while (++j < UI_STATE_AMOUNT)
		{
			if (elem->colors[j] != color)
				elem->texture_recreate = 1;
			elem->colors[j] = color;
		}
		return ;
	}
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
	elem->texture_recreate = 1;
}

void	ui_element_parent_set(t_ui_element *elem, t_ui_element *parent, int type, bool *show)
{
	elem->parent = parent;
	elem->parent_type = type;
	elem->parent_show = show;
}

void	ui_element_id_set(t_ui_element *elem, char *id)
{
	if (elem->id)
		ft_strdel(&elem->id);
	elem->id = ft_strdup(id);
}

void	ui_element_print(t_ui_element *elem)
{
	ft_printf("[%s]\n", __FUNCTION__);
	ft_printf("\tid : %s\n", elem->id);
	ft_printf("\tpos : ");
	print_vec(elem->pos.v, 4);
	ft_printf("\tscreen_pos : ");
	print_veci(elem->screen_pos.v, 4);
	ft_printf("\tparent_pos : ");
	if (elem->parent_type == UI_TYPE_WINDOW)
		print_veci(((t_ui_window *)elem->parent)->screen_pos.v, 4);
	else
		print_veci(((t_ui_element *)elem->parent)->screen_pos.v, 4);
	ft_printf("\tuse_images : %d\n", elem->use_images);
	ft_printf("\tcolors : %#x %#x %#x\n", elem->colors[0], elem->colors[1], elem->colors[2]);
	ft_printf("\tstate : %d\n", elem->state);
	ft_printf("\tparent_type : %d\n", elem->parent_type);
	ft_printf("\telement_type : %d\n", elem->element_type);
	ft_printf("\tshow : %d\n", elem->show);
	ft_printf("\tis_hover : %d\n", elem->is_hover);
	ft_printf("\tis_click : %d\n", elem->is_click);
	ft_printf("\ttexture_recreate : %d\n", elem->texture_recreate);
}
