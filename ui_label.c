#include "libui_v3.h"

void	ui_label_new(t_ui_window *win, t_ui_element *label)
{
	t_ui_label			*lab;

	ui_element_new(win, label);
	label->element = ft_memalloc(sizeof(t_ui_label));
	label->element_type = UI_TYPE_LABEL;
	lab = label->element;
	lab->text = ft_strdup("label text");
	lab->text_align = UI_TEXT_ALIGN_CENTER;
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

/*
 * Maybe only realign if you have updated the label.
*/
void	ui_label_text_align(t_ui_element *elem)
{
	t_ui_label	*label;

	label = elem->element;
	if (label->text_align == UI_TEXT_ALIGN_NONE)
		return ;
	if (label->text_align == UI_TEXT_ALIGN_CENTER)
		ui_label_text_center(elem);
	else
		ft_printf("[ui_label_text_align] Align type [%d] not supported.\n", label->text_align);
}

void	ui_label_render(t_ui_element *elem)
{
	t_vec4i		parent_pos;
	t_ui_label	*label;

	label = elem->element;
	if (elem->parent_type == UI_TYPE_WINDOW)
		parent_pos = ((t_ui_window *)elem->parent)->screen_pos;
	else
		parent_pos = ((t_ui_element *)elem->parent)->screen_pos;
	if (!*elem->parent_show || !elem->show)
		return ;
	elem->screen_pos.x = parent_pos.x + elem->pos.x;
	elem->screen_pos.y = parent_pos.y + elem->pos.y;

	if (label->texture_recreate)
		ui_label_texture_redo(elem);
	
	ui_label_text_align(elem);

	SDL_SetRenderTarget(elem->win->renderer, NULL);
	SDL_RenderCopy(elem->win->renderer, elem->textures[UI_STATE_DEFAULT], NULL,
		&(SDL_Rect){elem->screen_pos.x, elem->screen_pos.y, elem->pos.w, elem->pos.h});
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

/*
 * NOTE: if you set the label position manually, it will reset the text align.
*/
void	ui_label_pos_set(t_ui_element *elem, t_vec4i pos)
{
	t_ui_label	*label;

	label = elem->element;
	if (elem->pos.w != pos.w || elem->pos.h != pos.h)
		((t_ui_label *)elem->element)->texture_recreate = 1;
	elem->pos = pos;
	label->text_align = UI_TEXT_ALIGN_NONE;
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

void	ui_label_text_center(t_ui_element *elem)
{
	t_ui_label		*label;

	label = elem->element;
	if (!label || elem->element_type != UI_TYPE_LABEL || !label->font || !label->text)
	{
		if (!label)
			ft_printf("[ui_label_text_center] No label in element.\n");
		else if (elem->element_type != UI_TYPE_LABEL)
			ft_printf("[ui_label_text_center] Element not of type UI_TYPE_LABEL is %d.\n", elem->element_type);
		else
			ft_printf("[ui_label_text_center] One of the 15 warning you could get... come here and check.\n");
		return ;
	}
	if (elem->parent_type == UI_TYPE_WINDOW)
	{
		elem->pos.x = (((t_ui_window *)elem->parent)->pos.w / 2) - (label->text_wh.x / 2);	
		elem->pos.y = (((t_ui_window *)elem->parent)->pos.h / 2) - (label->text_wh.y / 2);	
	}
	else
	{
		elem->pos.x = (((t_ui_element *)elem->parent)->pos.w / 2) - (label->text_wh.x / 2);	
		elem->pos.y = (((t_ui_element *)elem->parent)->pos.h / 2) - (label->text_wh.y / 2);	
	}
	label->text_align = UI_TEXT_ALIGN_CENTER;
}

/*
 * End of editing functions
*/
void	ui_label_free(void *label)
{
	(void)label;
}
