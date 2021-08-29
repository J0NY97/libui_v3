#ifndef UI_ELEMENTS_H
# define UI_ELEMENTS_H
# include "libui_v3.h"
/*
 * Uint32		window_id;			the sdl window id that SDL_GetWindowID() returns;
 * t_vec2i		mouse_pos;			x, y for mouse on window texture;
 * t_vec2i		window_mouse_pos;	x, y for mouse taking into consideration window size vs. window texture size;
 * float		texture_scale;		the ratio between the window size and the window texture size (.x = w, .y = h);
 * t_vec4i		screen_pos;			this is the ACTUAL size of the window.
*/
typedef struct s_ui_window
{
	char			*id;
	Uint32			window_id;
	t_vec4			pos;
	t_vec4i			screen_pos;
	t_vec2			texture_scale;
	SDL_Window		*win;
	SDL_Renderer	*renderer;
	SDL_Texture		*texture;
	char			*title;
	t_vec2i			mouse_pos;
	t_vec2i			window_mouse_pos;
	bool			mouse_down;
	bool			show;
	bool			textures_recreate;
}					t_ui_window;

/*
 * t_vec4i		pos;						the position of the elem relative to its parent.
 * t_vec4i		screen_pos;					the position of the elem relative to the screen.
 * SDL_Texture	*states[UI_STATE_AMOUNT];	textures for all the different states there are.
 * SDL_Texture	*images[UI_STATE_AMOUNT];	textures for all the different state images there are.
 * int			state;						the state the element is in, enum t_element_states.
 * t_ui_window	*win;						the window you want the element on.
 * void			*parent;					the parent, controls show and screen_pos. should always be a t_ui_element
 * int			parent_type;				the type of the parent enum t_element_type.
 * t_vec4		*parent_screen_pos;			pointer to the actual parent->screen_pos, so we dont have to if it everywhere.
 * void			*element;					the real element.
 * int			element_type;				the type of the real element enum t_element_type.
 * bool			show;						wheather to render or not.
 * char			*id;						id of the element, so we can write a get_element_by_id();
 * bool			is_hover;					1 if the mouse is hovering over the element.
 * bool			is_click;					1 if you have mouse button down on the element. basically if hover && mouse_down.
*/
typedef struct s_ui_element
{
	t_vec4			pos;
	t_vec4i			screen_pos;
	SDL_Texture		*textures[UI_STATE_AMOUNT];
	SDL_Texture		*images[UI_STATE_AMOUNT];
	bool			use_images;
	int				state;
	t_ui_window		*win;
	void			*parent;
	int				parent_type;
	t_vec4i			*parent_screen_pos;
	bool			*parent_show;
	void			*element;
	int				element_type;
	bool			show;
	char			*id;
	bool			is_hover;
	bool			is_click;
	bool			texture_recreate;
	Uint32			colors[UI_STATE_AMOUNT];
}					t_ui_element;

/*
 * Description:
 * Menu is a compilation of elements, probably makes it easier to toggle on/off
 * a big group of elements, since children takes their ->show variable from their
 * parent element. (Thats atleast the point)
 *
 * t_list				*children;		list of elements of types e_element_type
*/
typedef struct s_ui_menu
{
	t_ui_element		*elem;
	t_list				*children;
}						t_ui_menu;

/*
 * bool	font_recreate;		set this to 1 if you edit something of the font like.. size. will be set to 0 after creation.
 * bool	texture_recreate;	set this to 1 if you edit something of the texture like.. anything.. will be set to 0 after creation.
 * t_vec2i	text_wh;		w and h of the whole text. (actual text not the element)
 * int		text_align;		one of enum e_ui_text_align.
*/
typedef struct s_ui_label
{
	char				*text;
	t_vec2i				text_wh;
	char				*font_path;
	unsigned int		font_size;
	Uint32				font_color;
	int					max_w;
	TTF_Font			*font;
	bool				font_recreate;
	bool				texture_recreate;
	int					text_align;
}						t_ui_label;

/*
 * t_ui_element		*elem;		actual elem, dont free.
*/
typedef struct s_ui_button
{
	t_ui_element		*elem;
	t_ui_element		label;
}						t_ui_button;

/*
 * NOTE: we treat dropdown as button.
*/
typedef struct s_ui_dropdown
{
	t_ui_element		*elem;
	t_ui_element		label;
	t_ui_element		menu;
}						t_ui_dropdown;

typedef struct s_ui_input
{
	t_ui_element		*elem;
	t_ui_element		label;
	int					cursor_on_char_num;
	int					cursor_from_char_num;
	int					cursor_on_char_x;
	int					cursor_from_char_x;
}						t_ui_input;

typedef struct s_ui_slider
{
	t_ui_element		*elem;
	t_ui_element		button;
	t_ui_element		min_label;
	t_ui_element		max_label;
	int					value;
	int					min_value;
	int					max_value;
	bool				update;
}						t_ui_slider;

/*
 * Basically a button that is toggle:able
*/
typedef struct s_ui_checkbox
{
	t_ui_element		*elem;
}						t_ui_checkbox;

/*
 * A collection of buttons where only one button can
 * be toggled at a time.
 *
 * Could probably use checkbox event on them.
 * But you have to untoggle all the other ones.
 *
 * t_list			*buttons;		list of t_ui_element with element = t_ui_button
*/
// Could probably be used in the tab buttons.
typedef struct s_ui_radio
{
	t_ui_element		*elem;
	t_list				*buttons;
	t_ui_element		*active;
}						t_ui_radio;
/*
 * If you click one of the buttons it will get toggled,
 * and its corresponding menu will be shown.
 * Only one button can be toggled at a time.
 *
 * t_ui_element	*active;		currently clicked button;
 * t_ui_element	*active_menu;	currently clicked button's menu;
 * t_list		*buttons;		list of t_ui_element with ->element = t_ui_button;
 * t_list		*menus;			list of t_ui_element with ->element = t_ui_menu;
*/
typedef struct s_ui_tab
{
	t_ui_element		*elem;
	t_ui_element		*active;
	t_ui_element		*active_menu;
	t_list				*buttons;
	t_list				*menus;
}						t_ui_tab;

// Window
void					ui_window_new(t_ui_window *win, char *title, t_vec4 pos);
void					ui_window_event(t_ui_window *win, SDL_Event e);
int						ui_window_render(t_ui_window *win);
void					ui_window_free(void *win);
void					ui_window_flag_set(t_ui_window *win, int flags);

// Element
void					ui_element_new(t_ui_window *win, t_ui_element *elem);
void					ui_element_textures_redo(t_ui_element *elem);
int						ui_element_render(t_ui_element *elem);
int						ui_element_is_hover(t_ui_element *elem);
int						ui_element_is_click(t_ui_element *elem);
void					ui_element_pos_set(t_ui_element *elem, t_vec4 pos);
void					ui_element_pos_set2(t_ui_element *elem, t_vec2 pos);
void					ui_element_color_set(t_ui_element *elem, int state, Uint32 color);
void					ui_element_image_set(t_ui_element *elem, int state, SDL_Surface *image);
void					ui_element_image_set_from_path(t_ui_element *elem, int state, char *image_path);
void					ui_element_parent_set(t_ui_element *elem, void *parent, int type);
void					ui_element_id_set(t_ui_element *elem, char *id);
void					ui_element_print(t_ui_element *elem);

// Label
void					ui_label_new(t_ui_window *win, t_ui_element *label);
void					ui_label_texture_redo(t_ui_element *label);
void					ui_label_event(t_ui_element *elem, SDL_Event e);
int						ui_label_render(t_ui_element *label);
void					ui_label_free(void *label);
// label other
void					ui_label_text_set(t_ui_element *label, char *text);
void					ui_label_font_set(t_ui_element *label, char *font_path);
void					ui_label_size_set(t_ui_element *label, size_t size);
void					ui_label_color_set(t_ui_element *label, Uint32 color);
void					ui_label_text_center(t_ui_element *elem);
void					ui_label_text_align(t_ui_element *elem, int align);
void					ui_label_print(t_ui_label *label);

// Button
void					ui_button_new(t_ui_window *win, t_ui_element *button);
void					ui_button_event(t_ui_element *button, SDL_Event e);
bool					ui_button(t_ui_element *button);
int						ui_button_render(t_ui_element *button);
void					ui_button_free(void *button);

// Menu
void					ui_menu_new(t_ui_window *win, t_ui_element *menu);
void					ui_menu_child_add(t_ui_element *menu, void *child, int type);
int						ui_menu_render(t_ui_element *menu);
void					ui_menu_event(t_ui_element *menu, SDL_Event e);
void					ui_menu_free(void *menu);

// Dropdown
void					ui_dropdown_new(t_ui_window *win, t_ui_element *drop);
void					ui_dropdown_event(t_ui_element *drop, SDL_Event e);
int						ui_dropdown_render(t_ui_element *drop);
void					ui_dropdown_free(void *drop);

// Input
void					ui_input_new(t_ui_window *win, t_ui_element *elem);
void					ui_input_event(t_ui_element *elem, SDL_Event e);
int						ui_input_render(t_ui_element *elem);
void					ui_input_free(void *elem);

// Slider
void					ui_slider_new(t_ui_window *win, t_ui_element *elem);
void					ui_slider_event(t_ui_element *elem, SDL_Event e);
int						ui_slider_render(t_ui_element *elem);
void					ui_slider_free(void *elem);

// Checkbox
void					ui_checkbox_new(t_ui_window *win, t_ui_element *elem);
void					ui_checkbox_event(t_ui_element *elem, SDL_Event e);
int						ui_checkbox_render(t_ui_element *elem);
void					ui_checkbox_free(void *elem);

// Radio
void					ui_radio_new(t_ui_window *win, t_ui_element *elem);
void					ui_radio_event(t_ui_element *elem, SDL_Event e);
int						ui_radio_render(t_ui_element *elem);
void					ui_radio_free(void *elem);
// Other radio
void					ui_radio_add(t_ui_element *elem, t_ui_element *child);

// Tab
void					ui_tab_new(t_ui_window *win, t_ui_element *elem);
void					ui_tab_event(t_ui_element *elem, SDL_Event e);
int						ui_tab_render(t_ui_element *elem);
void					ui_tab_free(void *elem);
// Other tab
void					ui_tab_add(t_ui_element *elem, t_ui_element *button, t_ui_element *menu);

#endif