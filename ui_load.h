#ifndef UI_LOAD_H
# define UI_LOAD_H
# include "libui.h"
/*
# include "ui_elements.h"
# include "ui_elements_v2.h"
# include "ui_enum.h"
# include "ui_vec.h"
*/

typedef struct s_ui_recipe		t_ui_recipe;
typedef struct s_ui_get			t_ui_get;
typedef struct s_ui_key_value	t_ui_key_value;
typedef struct s_ui_layout		t_ui_layout;

void	ui_button_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *args);
void	ui_label_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *args);
void	ui_menu_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *args);
void	ui_dropdown_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *args);
void	ui_input_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *args);
void	ui_slider_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *args);
void	ui_checkbox_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *args);
void	ui_radio_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *args);
void	ui_tab_editor(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *args);

void	ui_layout_element_edit(t_ui_element *elem, t_ui_recipe *recipe, t_ui_layout *layout);
void	ui_layout_element_new(t_ui_layout *layout, t_ui_window *win, t_ui_recipe *recipe);

t_ui_element	*ui_element_create_from_recipe(t_ui_window *win, t_ui_recipe *recipe, t_ui_layout *layout);

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
	char			*name;
	int				type;
	void			(*freer)(void *args);
	void			(*editor)(t_ui_element *elem, t_ui_recipe *child_recipe, t_ui_layout *args);
	void			(*maker)(t_ui_window *win, t_ui_element *elem);
	int				(*renderer)(t_ui_element *elem);
	void			(*eventer)(t_ui_element *elem, SDL_Event e);
	void			(*edit)(t_ui_element *elem, t_ui_recipe_v2 *recipe);
	t_ui_element	*(*getter)(t_ui_element *elem, int ui_element_type);
}				t_ui_acceptable;

static const t_ui_acceptable	g_acceptable_element =
{
	.name = "Element",
	.type = UI_TYPE_ELEMENT,
	.freer = NULL,
	.editor = NULL,
	.maker = NULL,
	.renderer = &ui_element_render,
	.eventer = &ui_element_event,
	.edit = &ui_element_edit,
	.getter = NULL
};

static const t_ui_acceptable	g_acceptable_button =
{
	.name = "Button",
	.type = UI_TYPE_BUTTON,
	.freer = &ui_button_free,
	.editor = &ui_button_editor,
	.maker = &ui_button_new,
	.renderer = &ui_button_render,
	.eventer = &ui_button_event,
	.edit = &ui_button_edit,
	.getter = NULL //&ui_button_get , button doesnt need this, you can acess the label stuff anyways.
};

static const t_ui_acceptable	g_acceptable_label =
{
	.name = "Label",
	.type = UI_TYPE_LABEL,
	.freer = &ui_label_free,
	.maker = &ui_label_new,
	.editor = &ui_label_editor,
	.renderer = &ui_label_render,
	.eventer = &ui_label_event,
	.edit = &ui_label_edit,
	.getter = NULL
};

static const t_ui_acceptable	g_acceptable_menu =
{
	.name = "Menu",
	.type = UI_TYPE_MENU,
	.freer = &ui_menu_free,
	.maker = &ui_menu_new,
	.editor = &ui_menu_editor,
	.renderer = &ui_menu_render,
	.eventer = &ui_menu_event,
	.edit = NULL,
	.getter = NULL
};

static const t_ui_acceptable	g_acceptable_dropdown =
{
	.name = "Dropdown",
	.type = UI_TYPE_DROPDOWN,
	.freer = &ui_dropdown_free,
	.maker = &ui_dropdown_new,
	.editor = &ui_dropdown_editor,
	.renderer = &ui_dropdown_render,
	.eventer = &ui_dropdown_event,
	.edit = &ui_dropdown_edit,
	.getter = &ui_dropdown_get
};

static const t_ui_acceptable	g_acceptable_input =
{
	.name = "Input",
	.type = UI_TYPE_INPUT,
	.freer = &ui_input_free,
	.maker = &ui_input_new,
	.editor = &ui_input_editor,
	.renderer = &ui_input_render,
	.eventer = &ui_input_event,
	.edit = &ui_input_edit,
	.getter = NULL
};

static const t_ui_acceptable	g_acceptable_slider =
{
	.name = "Slider",
	.type = UI_TYPE_SLIDER,
	.freer = &ui_slider_free,
	.maker = &ui_slider_new,
	.editor = &ui_slider_editor,
	.renderer = &ui_slider_render,
	.eventer = &ui_slider_event,
	.edit = &ui_slider_edit,
	.getter = &ui_slider_get
};

static const t_ui_acceptable	g_acceptable_checkbox =
{
	.name = "Checkbox",
	.type = UI_TYPE_CHECKBOX,
	.freer = &ui_checkbox_free,
	.maker = &ui_checkbox_new,
	.editor = &ui_checkbox_editor,
	.renderer = &ui_checkbox_render,
	.eventer = &ui_checkbox_event,
	.edit = NULL,
	.getter = NULL
};

static const t_ui_acceptable	g_acceptable_radio =
{
	.name = "Radio",
	.type = UI_TYPE_RADIO,
	.freer = &ui_radio_free,
	.maker = &ui_radio_new,
	.editor = &ui_radio_editor,
	.renderer = &ui_radio_render,
	.eventer = &ui_radio_event,
	.edit = NULL,
	.getter = NULL
};

static const t_ui_acceptable	g_acceptable_tab =
{
	.name = "Tab",
	.type = UI_TYPE_TAB,
	.freer = &ui_tab_free,
	.maker = &ui_tab_new,
	.editor = &ui_tab_editor,
	.renderer = &ui_tab_render,
	.eventer = &ui_tab_event,
	.edit = NULL,
	.getter = NULL
};

static const t_ui_acceptable	g_acceptable_window =
{
	.name = "Window",
	.type = UI_TYPE_WINDOW,
	.freer = NULL,
	.maker = NULL,
	.editor = NULL,
	.renderer = NULL,
	.eventer = NULL,
	.edit = NULL,
	.getter = NULL
};

// NOTE: these must be in the same order as the UI_TYPE_ENUM
static const t_ui_acceptable	g_acceptable[] =
{
	g_acceptable_element,
	g_acceptable_label,
	g_acceptable_button,
	g_acceptable_menu,
	g_acceptable_dropdown,
	g_acceptable_input,
	g_acceptable_slider,
	g_acceptable_checkbox,
	g_acceptable_radio,
	g_acceptable_tab,
	g_acceptable_window,
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

typedef struct s_ui_key_value
{
	char		*key;
	char		*value;
}				t_ui_key_value;

/*
 * int		type;		one of enum e_element_type
 * int		pos_info;	one or more of the enum e_pos_info
*/
struct s_ui_recipe
{
	char				*id;
	int					type;

	t_vec4				pos;	
	bool				pos_set;
	int					pos_info;

	Uint32				bg_color[UI_STATE_AMOUNT];
	bool				bg_color_set[UI_STATE_AMOUNT];

	char				*bg_image[UI_STATE_AMOUNT];
	bool				bg_image_set;

	int					flag;
	bool				flag_set;

	char				**children_ids;
	int					child_amount;
	t_list				*recipes;

	// Label Specific
	char				*title;

	char				*font_path;
	bool				font_path_set;

	Uint32				font_size;
	int					font_size_set;

	Uint32				font_color;
	bool				font_color_set;

	int					text_align;
	bool				text_align_set;

	// Slider Specific
	int					values[3];
	bool				value_set;

	// Tab specific stuff
	t_ui_key_value		tabs[256]; // key = button, value = menu
	int					tab_amount;
};

void	ui_layout_add_child(t_list **list, t_list *recipes, t_ui_window *win, t_ui_recipe *recipe);
#endif
