#include "libui_v3.h"

void	ui_label_new(t_ui_window *win, t_ui_label *label)
{
	memset(label, 0, sizeof(t_ui_label));
	ui_element_new(win, &label->elem);

	label->text_pos.x = label->elem.pos.x;
	label->text_pos.y = label->elem.pos.y;
	label->recipe.text = ft_strdup("default text");
//	label->recipe.font_path = ft_strdup(UI_FONT_PATH"DroidSans.ttf");
	label->recipe.font_path = ft_strdup("fonts/DroidSans.ttf");
	label->recipe.font_size = 12;
	label->recipe.font_color = 0xff9D858D;
	label->recipe.max_w = -1;
	label->text_texture = ui_texture_create_from_text_recipe(win->renderer, label->recipe);	
}

void	ui_label_render(t_ui_label *label)
{
	int	w;
	int	h;

	ui_element_render(&label->elem);
	SDL_QueryTexture(label->text_texture, NULL, NULL, &w, &h);
	SDL_SetRenderTarget(label->elem.win->renderer, NULL);
	SDL_RenderCopy(label->elem.win->renderer, label->text_texture, NULL,
		&(SDL_Rect){label->text_pos.x, label->text_pos.y, w, h});
}
