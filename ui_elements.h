#ifndef UI_ELEMENTS_H
# define UI_ELEMENTS_H
# include "libui.h"

/*
 * Uint32		window_id;			the sdl window id that SDL_GetWindowID() returns;
 * t_vec4i		pos;				the texture size of the window;
 * t_vec4		screen_pos;			the actual size of the window;
 * t_vec2i		mouse_pos;			x, y for mouse on window texture;
 * t_vec2i		window_mouse_pos;	x, y for mouse taking into consideration window size vs. window texture size;
 * t_vec2i		mouse_pos_prev;		last frame mouse pos on window;
 * int			mouse_down			button- left : 1, right: 3, middle : 2;
 * float		texture_scale;		the ratio between the window size and the window texture size (.x = w, .y = h);
 * t_vec4i		screen_pos;			this is the ACTUAL size of the window.
 * bool			wants_to_close;		is true when x on the window was clicked;
 * bool			hide_on_x;			if true when x is clicked, hide window (this and wants_to_close cant be set at the same time);
 * bool			user_handled_event;	true, when user has decided to event handle the window themselves; (so we dont do it twice); (used in the layout event handler);
 * t_ui_layout	*layout;			the layout it is part of, or if NULL it is not part of any layout;
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
	t_vec2i			mouse_pos_prev;
	t_vec2i			window_mouse_pos;
	int				scroll;
	int				mouse_down;
	int				mouse_down_last_frame;
	bool			show;
	bool			textures_recreate;
	bool			wants_to_close;
	bool			hide_on_x;
	bool			user_handled_event;
	t_list			*children;
	t_ui_layout		*layout;
	Uint32			bg_color;
}					t_ui_window;

/*
 * t_vec4i		pos;						the position of the elem relative to its parent.
 * t_vec4i		screen_pos;					the position of the elem with all the nested parents and children on the screen.
 * t_vec4i		from_pos;					the position of from what coordinates of the elem texture you want to render;
 * t_vec4i		to_pos;						the position of to what coordinates of the screen you want to render;
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
 * bool			is_click;					1 if you have mouse button down on the element. basically if hover && mouse_down;
 * bool			was_click;					1 if the element was clicked this frame; resets every time you event handle;
 * t_list		children;					list of t_ui_element;
 * int			z;							z value of the elements, aka render order;
*/
typedef struct s_ui_element
{
	t_vec4			pos;
	t_vec4i			from_pos;
	t_vec4i			to_pos;
	t_vec4i			screen_pos;
	SDL_Surface		*textures[UI_STATE_AMOUNT];
	SDL_Texture		*texture;
	SDL_Surface		*images[UI_STATE_AMOUNT];
	bool			use_images;
	int				state;
	int				last_state;
	t_ui_window		*win;
	void			*parent;
	int				parent_type;
	t_vec4i			*parent_screen_pos;
	bool			*parent_show;
	void			*element;
	int				element_type;
	t_list			*children;
	bool			show;
	char			*id;
	bool			is_hover;
	bool			is_click;
	bool			is_toggle;
	bool			was_click;
	bool			texture_recreate;
	Uint32			colors[UI_STATE_AMOUNT];
	bool			was_rendered_last_frame;
	bool			*parent_was_rendered_last_frame;
	bool			render_me_on_parent;
	int				z;
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
	bool				event_and_render_children;
}						t_ui_menu;

/*
 * Important Note:
 * when giving a font_path to the label,
 * we will try to search from the absolute path of the given str,
 * if not found, we will try to find the font in the libui fonts/ directory.
 *
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
 * bool			drop_exit;		will only return 1 the frame the menu was closed;
 * bool			drop_open;		will only return 1 the frame the menu was opened;
 * int			max_h;			if menu children total height exceeds the max_h a scrollbar will be shown;
*/
typedef struct s_ui_dropdown
{
	t_ui_element		*elem;
	t_ui_element		label;
	t_ui_element		menu;
	t_ui_element		*active;
	bool				drop_exit;
	bool				drop_open;
	t_ui_element		scrollbar;
	int					max_h;
}						t_ui_dropdown;

/*
 * Uint32	input_type;		0 : default/everything, 1: numbers, 2: letters;
 * bool		input_exit;		1 if you exit the inputting, aka enter or click somewhere else; (basically when you remove yourself from the inputting);
*/
typedef struct s_ui_input
{
	t_ui_element		*elem;
	t_ui_element		label;
	t_ui_element		placeholder;
	int					cursor_on_char_num;
	int					cursor_from_char_num;
	int					cursor_on_char_x;
	int					cursor_from_char_x;
	Uint32				input_type;
	bool				input_exit;
}						t_ui_input;

typedef struct s_ui_slider
{
	t_ui_element		*elem;
	t_ui_element		button;
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
*/
// Could probably be used in the tab buttons.
/*
 * Maybe reintroduce the group idea you had, in radio you just have buttons in the list, if youre using it in the
 * tab you have button and menu, and the only thing you do on the group is event handling and ofc render...,
 * so they^1 dont have to be even elements. *1 they = tab and radio;
*/
typedef struct s_ui_radio
{
	t_ui_element		*elem;
	t_ui_element		*active;
}						t_ui_radio;

/*
 * Description:
 * If you click one of the buttons it will get toggled,
 * and its corresponding menu will be shown.
 * Only one button can be toggled at a time.
 *
 * NOTE:
 * By default we are setting the tab button and menu as the parent of the tab elem,
 * if you want to set the buttons where you want, without the element_render pulling
 * the button and menu elements back to the tab elem, you can remove the parent from
 * the button and menu elem (or set the parent to the window).
 * I hope you understood, 'cause I don't.
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

/*
 * Slider event:ish but vertical, plus it will move the position of the target children up or down;
 *
 * void		*target;		the element on which the event will happen on;
 * int		target_type;	the type of element, either UI_TYPE_ELEMENT OR -_WINDOW;
 * int		target_size;	the size of all children elements of target together;
 * bool		update;			has been updated this frame;
*/
typedef struct s_ui_scrollbar
{
	t_ui_element		button;
	void				*target;
	int					target_type;
	int					target_size;
	int					last_value;
	int					value;
	int					min;
	int					max;
	bool				update;
	t_vec2i				top_most;
	t_vec2i				bot_most;
}						t_ui_scrollbar;

// Window
void					ui_window_new(t_ui_window *win, char *title, t_vec4 pos);
void					ui_window_event(t_ui_window *win, SDL_Event e);
int						ui_window_render(t_ui_window *win);
void					ui_window_free(void *win);
void					ui_window_edit(t_ui_window *win, t_ui_recipe *recipe);
// Window other
void					ui_window_texture_redo(t_ui_window *win);
void					ui_window_pos_set(t_ui_window *win, t_vec4 pos);
void					ui_window_flag_set(t_ui_window *win, int flags);
void					ui_window_title_set(t_ui_window *win, const char *title);
void					ui_window_id_set(t_ui_window *win, const char *id);
void					ui_window_replace_win(t_ui_window *ui_win, SDL_Window *sdl_win);

// Element
void					ui_element_new(t_ui_window *win, t_ui_element *elem);
void					ui_element_free(t_ui_element *elem);
void					ui_element_event(t_ui_element *elem, SDL_Event e);
void					ui_element_edit(t_ui_element *elem, t_ui_recipe *recipe);
void					ui_element_textures_redo(t_ui_element *elem);
int						ui_element_render(t_ui_element *elem);
int						ui_element_is_hover(t_ui_element *elem);
int						ui_element_is_click(t_ui_element *elem);
void					ui_element_pos_set(t_ui_element *elem, t_vec4 pos);
void					ui_element_pos_set2(t_ui_element *elem, t_vec2 pos);
void					ui_element_color_set(t_ui_element *elem, int state, Uint32 color);
void					ui_element_image_set(t_ui_element *elem, int state, SDL_Surface *image);
void					ui_element_image_set_from_path(t_ui_element *elem, int state, char *image_path);
void					ui_element_remove_child_from_parent(t_ui_element *elem);
void					ui_element_set_parent(t_ui_element *elem, void *parent, int type);
void					ui_element_set_id(t_ui_element *elem, char *id);
void					ui_element_print(t_ui_element *elem);
t_vec4i					ui_element_screen_pos_get(t_ui_element *elem);
void					ui_element_swap(t_ui_element *one, t_ui_element *two);
int						ui_element_type_from_string(char *str);
const char				*ui_element_type_to_string(int type);
void					ui_element_move_list(t_list *list, t_vec2i amount);
void					ui_element_remove_from_list(t_ui_element *elem, t_list **list);

// Label
void					ui_label_new(t_ui_window *win, t_ui_element *label);
void					ui_label_edit(t_ui_element *elem, t_ui_recipe *recipe);
void					ui_label_texture_redo(t_ui_element *label);
void					ui_label_event(t_ui_element *elem, SDL_Event e);
int						ui_label_render(t_ui_element *label);
void					ui_label_print(t_ui_element *elem);
void					ui_label_free(void *label);
// label other
void					ui_label_set_text(t_ui_element *label, char *text);
void					ui_label_font_set(t_ui_element *label, char *font_path);
void					ui_label_size_set(t_ui_element *label, size_t size);
void					ui_label_color_set(t_ui_element *label, Uint32 color);
void					ui_label_text_center(t_ui_element *elem);
void					ui_label_text_align(t_ui_element *elem, int align);
// Getters
t_ui_label				*ui_label_get(t_ui_element *elem);
char					*ui_label_get_text(t_ui_element *elem);

// Button
void					ui_button_new(t_ui_window *win, t_ui_element *button);
void					ui_button_edit(t_ui_element *elem, t_ui_recipe *recipe);
void					ui_button_event(t_ui_element *button, SDL_Event e);
bool					ui_button(t_ui_element *button);
int						ui_button_render(t_ui_element *button);
void					ui_button_free(void *button);
t_ui_element			*ui_button_get(t_ui_element *elem, int ui_type);
// Getters
t_ui_element			*ui_button_get_label_element(t_ui_element *elem);
t_ui_label				*ui_button_get_label(t_ui_element *elem);
char					*ui_button_get_text(t_ui_element *elem);
t_ui_element			*ui_list_get_button_with_text(t_list *list, const char *str);
// Setters
void					ui_button_set_text(t_ui_element *elem, char *str);

// Menu
void					ui_menu_new(t_ui_window *win, t_ui_element *menu);
int						ui_menu_render(t_ui_element *menu);
void					ui_menu_event(t_ui_element *menu, SDL_Event e);
void					ui_menu_edit(t_ui_element *elem, t_ui_recipe *recipe);
void					ui_menu_free(void *menu);
// getters
t_ui_menu				*ui_menu_get_menu(t_ui_element *elem);

// Dropdown
void					ui_dropdown_new(t_ui_window *win, t_ui_element *drop);
void					ui_dropdown_edit(t_ui_element *elem, t_ui_recipe *recipe);
void					ui_dropdown_event(t_ui_element *drop, SDL_Event e);
int						ui_dropdown_render(t_ui_element *drop);
void					ui_dropdown_free(void *drop);
void					ui_dropdown_activate(t_ui_element *drop, t_ui_element *elem);
int						ui_dropdown_open(t_ui_element *elem);
int						ui_dropdown_exit(t_ui_element *elem);
t_ui_element			*ui_dropdown_get(t_ui_element *elem, int ui_type);
// Getters
t_ui_element			*ui_dropdown_get_button_element(t_ui_element *elem);
t_ui_element			*ui_dropdown_get_menu_element(t_ui_element *elem);
t_ui_dropdown			*ui_dropdown_get_dropdown(t_ui_element *elem);
t_ui_menu				*ui_dropdown_get_menu(t_ui_element *elem);
t_ui_element			*ui_dropdown_active(t_ui_element *elem);
char					*ui_dropdown_active_text(t_ui_element *elem);

// Input
void					ui_input_new(t_ui_window *win, t_ui_element *elem);
void					ui_input_edit(t_ui_element *elem, t_ui_recipe *recipe);
void					ui_input_event(t_ui_element *elem, SDL_Event e);
int						ui_input_render(t_ui_element *elem);
void					ui_input_free(void *elem);
t_ui_input				*ui_input_get(t_ui_element *elem);
t_ui_element			*ui_input_get_label_element(t_ui_element *elem);
t_ui_label				*ui_input_get_label(t_ui_element *elem);
char					*ui_input_get_text(t_ui_element *elem);
char					*ui_input_set_text(t_ui_element *elem, char *str);
char					*ui_input_set_placeholder_text(t_ui_element *elem, char *str);
int						ui_input_exit(t_ui_element *elem);

// Slider
void					ui_slider_new(t_ui_window *win, t_ui_element *elem);
void					ui_slider_edit(t_ui_element *elem, t_ui_recipe *recipe);
void					ui_slider_event(t_ui_element *elem, SDL_Event e);
int						ui_slider_render(t_ui_element *elem);
void					ui_slider_free(void *elem);
t_ui_element			*ui_slider_get(t_ui_element *elem, int ui_type);
// Other
int						ui_slider_updated(t_ui_element *elem);
// Getters
t_ui_element			*ui_slider_get_button_element(t_ui_element *elem);
t_ui_slider				*ui_slider_get_slider(t_ui_element *elem);
// slider edit
int						ui_slider_value_get(t_ui_element *elem);
void					ui_slider_value_set(t_ui_element *elem, int value);
// Internal
int						ui_get_slider_value(int min, int max, int pos_x, int w);
int						ui_set_slider_value(int value, int min, int max, int w);

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
void					ui_radio_button_toggle_on(t_ui_element *elem, t_ui_element *toggle_this);
int						ui_list_radio_event(t_list *list, t_ui_element **active);

// Tab
void					ui_tab_new(t_ui_window *win, t_ui_element *elem);
void					ui_tab_event(t_ui_element *elem, SDL_Event e);
int						ui_tab_render(t_ui_element *elem);
void					ui_tab_free(void *elem);
// Other tab
void					ui_tab_add(t_ui_element *elem, t_ui_element *button, t_ui_element *menu);

// Scrollbar
void					ui_scrollbar_new(t_ui_window *win, t_ui_element *elem);
void					ui_scrollbar_event(t_ui_element *elem, SDL_Event e);
int						ui_scrollbar_render(t_ui_element *elem);
void					ui_scrollbar_free(void *args);
void					ui_scrollbar_edit(t_ui_element *elem, t_ui_recipe *recipe);
// Other
void					ui_scroll_value_set(t_ui_element *elem, int value);
void					ui_scrollbar_recount(t_ui_element *elem);
// Get
t_ui_button				*ui_scrollbar_get_button(t_ui_element *elem);
t_ui_element			*ui_scrollbar_get_button_element(t_ui_element *elem);

#endif
