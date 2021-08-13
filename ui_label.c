#include "libui_v3.h"

void	ui_label_new(t_ui_window *win, t_ui_label *label)
{
	memset(label, 0, sizeof(t_ui_label));
	label->win = win;
	label->pos.x = 10;
	label->pos.y = 10;
	label->recipe.text = ft_strdup("default text");
//	label->recipe.font_path = ft_strdup(UI_FONT_PATH"DroidSans.ttf");
	label->recipe.font_path = ft_strdup("fonts/DroidSans.ttf");
	label->recipe.font_size = 12;
	label->recipe.font_color = 0xff037171;
	label->recipe.max_w = -1;
	ui_label_texture_redo(label);
}

void	ui_label_texture_redo(t_ui_label *label)
{
	if (label->texture)
		SDL_DestroyTexture(label->texture);
	label->texture = ui_texture_create_from_text_recipe(label->win->renderer, label->recipe);	
	SDL_QueryTexture(label->texture, NULL, NULL, &label->pos.w, &label->pos.h);
}

void	ui_label_pos_set(t_ui_label *label, t_vec4i pos)
{
	int	redo;

	redo = 0;
	if (label->pos.w != pos.w || label->pos.h != pos.h)
		redo = 1;
	label->pos = pos;
	if (redo)
		ui_label_texture_redo(label);
}

void	ui_label_render(t_ui_label *label)
{
	SDL_SetRenderTarget(label->win->renderer, NULL);
	SDL_RenderCopy(label->win->renderer, label->texture, NULL,
		&(SDL_Rect){label->pos.x, label->pos.y, label->pos.w, label->pos.h});
}
