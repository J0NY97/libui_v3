#ifndef UI_LOAD_H
# define UI_LOAD_H
# include "libui_v3.h"
# include "ui_enum.h"
# include "ui_vec.h"

typedef struct s_ui_recipe		t_ui_recipe;
typedef struct s_ui_get			t_ui_get;
typedef struct s_ui_key_value	t_ui_key_value;

typedef struct s_ui_window		t_ui_window;
typedef struct s_ui_label		t_ui_label;
typedef struct s_ui_button		t_ui_button;
typedef struct s_ui_dropdown	t_ui_dropdown;
typedef struct s_ui_element		t_ui_element;
typedef struct s_ui_layout		t_ui_layout;

void	ui_window_new(t_ui_window *win, char *title, t_vec4i pos);
int		ui_window_render(t_ui_window *win);
void	ui_window_free(void *win);

void	ui_button_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *args);
void	ui_button_free(void *button);
void	ui_button_new(t_ui_window *win, t_ui_element *button);
int		ui_button_render(t_ui_element *button);
void	ui_button_event(t_ui_element *elem, SDL_Event e);

void	ui_label_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *args);
void	ui_label_free(void *label);
void	ui_label_new(t_ui_window *win, t_ui_element *label);
void	ui_label_event(t_ui_element *elem, SDL_Event e);
int		ui_label_render(t_ui_element *label);

void	ui_menu_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *args);
void	ui_menu_new(t_ui_window *win, t_ui_element *menu);
void	ui_menu_free(void *menu);
int		ui_menu_render(t_ui_element *menu);
void	ui_menu_event(t_ui_element *elem, SDL_Event e);

void	ui_dropdown_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *args);
int		ui_dropdown_render(t_ui_element *drop);
void	ui_dropdown_free(void *drop);
void	ui_dropdown_new(t_ui_window *win, t_ui_element *drop);
void	ui_dropdown_event(t_ui_element *elem, SDL_Event e);

void	ui_input_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *args);
int		ui_input_render(t_ui_element *elem);
void	ui_input_free(void *drop);
void	ui_input_new(t_ui_window *win, t_ui_element *elem);
void	ui_input_event(t_ui_element *elem, SDL_Event e);

void	ui_slider_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *args);
int		ui_slider_render(t_ui_element *elem);
void	ui_slider_free(void *drop);
void	ui_slider_new(t_ui_window *win, t_ui_element *elem);
void	ui_slider_event(t_ui_element *elem, SDL_Event e);

void	ui_checkbox_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *args);
void	ui_checkbox_new(t_ui_window *win, t_ui_element *elem);
void	ui_checkbox_event(t_ui_element *elem, SDL_Event e);
int		ui_checkbox_render(t_ui_element *elem);
void	ui_checkbox_free(void *elem);

void	ui_layout_element_edit(t_ui_element *elem, t_ui_recipe *recipe);
void	ui_layout_element_new(t_ui_layout *layout, t_ui_window *win, t_ui_recipe *recipe, t_list *recipes);

int		ui_element_render(t_ui_element *elem);

struct s_ui_get
{
	int				type;
	int				*len; // len of kv
	t_ui_key_value	*kv;
	t_ui_recipe		*recipe;
};

/*
 * Fun fact: you can only have flexible array in the end of a struct.
 * 	which actually makes sense if you think about it.
*/
typedef struct s_ui_acceptable
{
	char		*name;
	int			type;
	void		(*freer)(void *args);
	void		(*editor)(t_ui_element *elem, t_ui_recipe *child_recipe, t_ui_layout *args);
	void		(*maker)(t_ui_window *win, t_ui_element *elem);
	int			(*renderer)(t_ui_element *elem);
	void		(*eventer)(t_ui_element *elem, SDL_Event e);
}				t_ui_acceptable;

static const t_ui_acceptable	g_acceptable_element =
{
	.name = "Element",
	.type = UI_TYPE_ELEMENT,
	.freer = NULL,
	.editor = NULL,
	.maker = NULL,
	.renderer = &ui_element_render,
	.eventer = NULL 
};

static const t_ui_acceptable	g_acceptable_button =
{
	.name = "Button",
	.type = UI_TYPE_BUTTON,
	.freer = &ui_button_free,
	.editor = &ui_button_editor,
	.maker = &ui_button_new,
	.renderer = &ui_button_render,
	.eventer = &ui_button_event
};

static const t_ui_acceptable	g_acceptable_label =
{
	.name = "Label",
	.type = UI_TYPE_LABEL,
	.freer = &ui_label_free,
	.maker = &ui_label_new,
	.editor = &ui_label_editor,
	.renderer = &ui_label_render,
	.eventer = &ui_label_event
};

static const t_ui_acceptable	g_acceptable_menu =
{
	.name = "Menu",
	.type = UI_TYPE_MENU,
	.freer = &ui_menu_free,
	.maker = &ui_menu_new,
	.editor = &ui_menu_editor,
	.renderer = &ui_menu_render,
	.eventer = &ui_menu_event
};

static const t_ui_acceptable	g_acceptable_dropdown =
{
	.name = "Dropdown",
	.type = UI_TYPE_DROPDOWN,
	.freer = &ui_dropdown_free,
	.maker = &ui_dropdown_new,
	.editor = &ui_dropdown_editor,
	.renderer = &ui_dropdown_render,
	.eventer = &ui_dropdown_event
};

static const t_ui_acceptable	g_acceptable_input =
{
	.name = "Input",
	.type = UI_TYPE_INPUT,
	.freer = &ui_input_free,
	.maker = &ui_input_new,
	.editor = &ui_input_editor,
	.renderer = &ui_input_render,
	.eventer = &ui_input_event
};

static const t_ui_acceptable	g_acceptable_slider =
{
	.name = "Slider",
	.type = UI_TYPE_SLIDER,
	.freer = &ui_slider_free,
	.maker = &ui_slider_new,
	.editor = &ui_slider_editor,
	.renderer = &ui_slider_render,
	.eventer = &ui_slider_event
};

static const t_ui_acceptable	g_acceptable_checkbox =
{
	.name = "Checkbox",
	.type = UI_TYPE_CHECKBOX,
	.freer = &ui_checkbox_free,
	.maker = &ui_checkbox_new,
	.editor = &ui_checkbox_editor,
	.renderer = &ui_checkbox_render,
	.eventer = &ui_checkbox_event
};

static const t_ui_acceptable	g_acceptable_window =
{
	.name = "Window",
	.type = UI_TYPE_WINDOW,
	.freer = NULL,
	.maker = NULL,
	.editor = NULL,
	.renderer = NULL,
	.eventer = NULL
};

# define UI_ACCEPT_AMOUNT 9
static const t_ui_acceptable	g_acceptable[] =
{
	g_acceptable_element,
	g_acceptable_button,
	g_acceptable_label,
	g_acceptable_menu,
	g_acceptable_window,
	g_acceptable_dropdown,
	g_acceptable_input,
	g_acceptable_slider,
	g_acceptable_checkbox,
	NULL
};

enum e_pos_info
{
	UI_POS_NONE = 0x00,
	UI_POS_X = 0x000001,
	UI_POS_Y = 0x000010,
	UI_POS_W = 0x000100,
	UI_POS_H = 0x001000
};


/*
 * int		type;		one of enum e_element_type
 * int		pos_info;	one or more of the enum e_pos_info
*/
struct s_ui_recipe
{
	char				*id;
	int					type;

	t_vec4i				pos;	
	bool				pos_set;
	int					pos_info;

	Uint32				bg_color[UI_STATE_AMOUNT];
	bool				bg_color_set;

	char				*bg_image[UI_STATE_AMOUNT];
	bool				bg_image_set;

	char				**children_ids;
	int					child_amount;
	t_list				*recipes;

	// Label Specific
	char				*title;

	char				*font_path;
	bool				font_path_set;

	Uint32				font_size;
	bool				font_size_set;

	Uint32				font_color;
	bool				font_color_set;

	int					text_align;
	bool				text_align_set;

	// Slider Specific
	int					values[3];
	bool				value_set;
};

typedef struct s_ui_key_value
{
	char		*key;
	char		*value;
}				t_ui_key_value;

void	ui_layout_add_child(t_list **list, t_list *recipes, t_ui_window *win, t_ui_recipe *recipe);
#endif
