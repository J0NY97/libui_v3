#include "libui_v3.h"

void	ui_label_new(t_ui_window *win, t_ui_label *label)
{
	memset(label, 0, sizeof(t_ui_label));
	label->parent = win;
	label->parent_type = UI_TYPE_WINDOW;
	label->parent_show = &win->show;
	label->win = win;
	label->pos.x = 0;
	label->pos.y = 0;
	label->recipe.text = ft_strdup("default text");
//	label->recipe.font_path = ft_strdup(UI_FONT_PATH"DroidSans.ttf");
	label->recipe.font_path = ft_strdup("fonts/DroidSans.ttf");
	label->recipe.font_size = 12;
	label->recipe.font_color = 0xff037171;
	label->recipe.max_w = -1;
	label->recipe.font = NULL;
	label->recipe.font_recreate = 1;
	label->recipe.texture_recreate = 1;
	ui_label_texture_redo(label);
	label->show = 1;
}

void	ui_label_texture_redo(t_ui_label *label)
{
	if (label->texture)
		SDL_DestroyTexture(label->texture);
	label->texture = ui_texture_create_from_text_recipe(label->win->renderer, &label->recipe);	
	SDL_QueryTexture(label->texture, NULL, NULL, &label->pos.w, &label->pos.h);
	label->recipe.texture_recreate = 0;
}

void	ui_label_render(t_ui_label *label)
{
	t_vec4i parent_pos;

	if (label->parent_type == UI_TYPE_WINDOW)
		parent_pos = ((t_ui_window *)label->parent)->screen_pos;
	else
		parent_pos = ((t_ui_element *)label->parent)->screen_pos;
	if (!*label->parent_show || !label->show)
		return ;
	label->screen_pos.x = parent_pos.x + label->pos.x;
	label->screen_pos.y = parent_pos.y + label->pos.y;

	if (label->recipe.texture_recreate)
		ui_label_texture_redo(label);

	SDL_SetRenderTarget(label->win->renderer, NULL);
	SDL_RenderCopy(label->win->renderer, label->texture, NULL,
		&(SDL_Rect){label->screen_pos.x, label->screen_pos.y, label->pos.w, label->pos.h});
}

/*
 * Every function under this is for editing the label.
*/
void	ui_label_text_set(t_ui_label *label, char *text)
{
	if (label->recipe.text)
		ft_strdel(&label->recipe.text);
	label->recipe.text = ft_strdup(text);
	label->recipe.texture_recreate = 1;
}

void	ui_label_font_set(t_ui_label *label, char *font_path)
{
	if (label->recipe.font_path)
		ft_strdel(&label->recipe.font_path);
	label->recipe.font_path = ft_strdup(font_path);
	label->recipe.font_recreate = 1;
}

void	ui_label_pos_set(t_ui_label *label, t_vec4i pos)
{
	if (label->pos.w != pos.w || label->pos.h != pos.h)
		label->recipe.texture_recreate = 1;
	label->pos = pos;
}

void	ui_label_size_set(t_ui_label *label, size_t size)
{
	if (label->recipe.font_size != size)
	{
		label->recipe.font_recreate = 1;
		label->recipe.texture_recreate = 1;
	}
	label->recipe.font_size = size;
}

void	ui_label_color_set(t_ui_label *label, Uint32 color)
{
	if (label->recipe.font_color != color)
		label->recipe.texture_recreate = 1;
	label->recipe.font_color = color;
}

void	ui_label_parent_set(t_ui_label *label, t_ui_element *parent, int type, bool *show)
{
	label->parent = parent;
	label->parent_type = type;
	label->parent_show = show;
}
/*
 * End of editing functions
*/
void	ui_label_free(void *label)
{
	(void)label;
}
