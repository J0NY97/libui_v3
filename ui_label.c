#include "libui_v3.h"

void	ui_label_new(t_ui_window *win, t_ui_element *label)
{
	t_ui_text_recipe	*recipe;
	t_ui_label			*lab;

	ui_element_new(win, label);
	label->element = ft_memalloc(sizeof(t_ui_label));
	label->element_type = UI_TYPE_LABEL;
	lab = label->element;
	recipe = &lab->recipe;
	recipe->text = ft_strdup("default text");
	recipe->font_path = ft_strdup("fonts/DroidSans.ttf");
	recipe->font_size = 12;
	recipe->font_color = 0xff037171;
	recipe->max_w = -1;
	recipe->font = NULL;
	recipe->font_recreate = 1;
	recipe->texture_recreate = 1;
	ui_label_texture_redo(label);
}

void	ui_label_texture_redo(t_ui_element *label)
{
	if (label->textures[UI_STATE_DEFAULT])
		SDL_DestroyTexture(label->textures[UI_STATE_DEFAULT]);
	label->textures[UI_STATE_DEFAULT] = ui_texture_create_from_text_recipe(label->win->renderer, &((t_ui_label *)label->element)->recipe);	
	SDL_QueryTexture(label->textures[UI_STATE_DEFAULT], NULL, NULL, &label->pos.w, &label->pos.h);
	((t_ui_label *)label->element)->recipe.texture_recreate = 0;
}

void	ui_label_render(t_ui_element *label)
{
	t_vec4i				parent_pos;
	t_ui_text_recipe	*recipe;
	t_ui_label			*lab;

	lab = label->element;
	recipe = &lab->recipe;
	if (label->parent_type == UI_TYPE_WINDOW)
		parent_pos = ((t_ui_window *)label->parent)->screen_pos;
	else
		parent_pos = ((t_ui_element *)label->parent)->screen_pos;
	if (!*label->parent_show || !label->show)
		return ;
	label->screen_pos.x = parent_pos.x + label->pos.x;
	label->screen_pos.y = parent_pos.y + label->pos.y;

	if (recipe->texture_recreate)
		ui_label_texture_redo(label);

	SDL_SetRenderTarget(label->win->renderer, NULL);
	SDL_RenderCopy(label->win->renderer, label->textures[UI_STATE_DEFAULT], NULL,
		&(SDL_Rect){label->screen_pos.x, label->screen_pos.y, label->pos.w, label->pos.h});
}

/*
 * Every function under this is for editing the label.
*/
void	ui_label_text_set(t_ui_element *label, char *text)
{
	t_ui_text_recipe	*recipe;

	recipe = &((t_ui_label *)label->element)->recipe;
	if (recipe->text)
		ft_strdel(&recipe->text);
	recipe->text = ft_strdup(text);
	recipe->texture_recreate = 1;
}

void	ui_label_font_set(t_ui_element *label, char *font_path)
{
	t_ui_text_recipe	*recipe;

	recipe = &((t_ui_label *)label->element)->recipe;
	if (recipe->font_path)
		ft_strdel(&recipe->font_path);
	recipe->font_path = ft_strdup(font_path);
	recipe->font_recreate = 1;
}

void	ui_label_pos_set(t_ui_element *label, t_vec4i pos)
{
	if (label->pos.w != pos.w || label->pos.h != pos.h)
		((t_ui_label *)label->element)->recipe.texture_recreate = 1;
	label->pos = pos;
}

void	ui_label_size_set(t_ui_element *label, size_t size)
{
	t_ui_text_recipe	*recipe;

	recipe = &((t_ui_label *)label->element)->recipe;
	if (recipe->font_size != size)
	{
		recipe->font_recreate = 1;
		recipe->texture_recreate = 1;
	}
	recipe->font_size = size;
}

void	ui_label_color_set(t_ui_element *label, Uint32 color)
{
	t_ui_text_recipe	*recipe;

	recipe = &((t_ui_label *)label->element)->recipe;
	if (recipe->font_color != color)
		recipe->texture_recreate = 1;
	recipe->font_color = color;
}

// TODO: dont use this.
void	ui_label_parent_set(t_ui_element *label, t_ui_element *parent, int type, bool *show)
{
	ui_element_parent_set(label, parent, type, show);
}

/*
 * End of editing functions
*/
void	ui_label_free(void *label)
{
	(void)label;
}
