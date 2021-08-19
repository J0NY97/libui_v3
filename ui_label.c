#include "libui_v3.h"

void	ui_label_new(t_ui_window *win, t_ui_element *label)
{
	t_ui_label			*lab;

	ui_element_new(win, label);
	label->element = ft_memalloc(sizeof(t_ui_label));
	label->element_type = UI_TYPE_LABEL;
	lab = label->element;
	lab->text = ft_strdup("default text");
	lab->font_path = ft_strdup("fonts/DroidSans.ttf");
	lab->font_size = 12;
	lab->font_color = 0xff037171;
	lab->max_w = -1;
	lab->font = NULL;
	lab->font_recreate = 1;
	lab->texture_recreate = 1;
	ui_label_texture_redo(label);
}

void	ui_label_texture_redo(t_ui_element *label)
{
	if (label->textures[UI_STATE_DEFAULT])
		SDL_DestroyTexture(label->textures[UI_STATE_DEFAULT]);
	label->textures[UI_STATE_DEFAULT] = ui_texture_create_from_text_recipe(label->win->renderer, label->element);	
	SDL_QueryTexture(label->textures[UI_STATE_DEFAULT], NULL, NULL, &label->pos.w, &label->pos.h);
	((t_ui_label *)label->element)->texture_recreate = 0;
}

void	ui_label_render(t_ui_element *label)
{
	t_vec4i				parent_pos;
	t_ui_label			*lab;

	lab = label->element;
	if (label->parent_type == UI_TYPE_WINDOW)
		parent_pos = ((t_ui_window *)label->parent)->screen_pos;
	else
		parent_pos = ((t_ui_element *)label->parent)->screen_pos;
	if (!*label->parent_show || !label->show)
		return ;
	label->screen_pos.x = parent_pos.x + label->pos.x;
	label->screen_pos.y = parent_pos.y + label->pos.y;

	if (lab->texture_recreate)
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
	t_ui_label	*lab;

	lab = label->element;
	if (lab->text)
		ft_strdel(&lab->text);
	lab->text = ft_strdup(text);
	lab->texture_recreate = 1;
}

void	ui_label_font_set(t_ui_element *label, char *font_path)
{
	t_ui_label	*lab;

	lab = label->element;
	if (lab->font_path)
		ft_strdel(&lab->font_path);
	lab->font_path = ft_strdup(font_path);
	lab->font_recreate = 1;
}

void	ui_label_pos_set(t_ui_element *label, t_vec4i pos)
{
	if (label->pos.w != pos.w || label->pos.h != pos.h)
		((t_ui_label *)label->element)->texture_recreate = 1;
	label->pos = pos;
}

void	ui_label_size_set(t_ui_element *label, size_t size)
{
	t_ui_label	*lab;

	lab = label->element;
	if (lab->font_size != size)
	{
		lab->font_recreate = 1;
		lab->texture_recreate = 1;
	}
	lab->font_size = size;
}

void	ui_label_color_set(t_ui_element *label, Uint32 color)
{
	t_ui_label	*lab;

	lab = label->element;
	if (lab->font_color != color)
		lab->texture_recreate = 1;
	lab->font_color = color;
}

/*
 * End of editing functions
*/
void	ui_label_free(void *label)
{
	(void)label;
}
