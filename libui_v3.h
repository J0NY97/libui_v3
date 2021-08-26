#ifndef LIBUI_V3_H
# define LIBUI_V3_H
# define SDL_MAIN_HANDLED
# include "SDL.h"
# include "SDL_image.h"
# include "SDL_ttf.h"
# include "libft.h"
# include "libpf.h"
# include "stdio.h"
# include "ui_enum.h"
# include "ui_vec.h"
# include "ui_load.h"
//# include "path.h"

typedef struct s_rgba
{
	union {
		struct {
			int r;
			int g;
			int b;
			int a;
		};
		int rgba[4];
	};
}					t_rgba;

typedef struct s_ui_window
{
	char			*id;
	t_vec4i			pos;
	t_vec4i			screen_pos;
	SDL_Window		*win;
	SDL_Renderer	*renderer;
	SDL_Texture		*texture;
	char			*title;
	t_vec2i			mouse_pos;
	bool			mouse_down;
	bool			show;
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
 * void			*element;					the real element.
 * int			element_type;				the type of the real element enum t_element_type.
 * bool			show;						wheather to render or not.
 * char			*id;						id of the element, so we can write a get_element_by_id();
 * bool			is_hover;					1 if the mouse is hovering over the element.
 * bool			is_click;					1 if you have mouse button down on the element. basically if hover && mouse_down.
 * bool			is_click_repeat;			1 if you have had mouse down on element for more than 1 frame.
*/
typedef struct s_ui_element
{
	t_vec4i			pos;
	t_vec4i			screen_pos;
	SDL_Texture		*textures[UI_STATE_AMOUNT];
	SDL_Texture		*images[UI_STATE_AMOUNT];
	bool			use_images;
	int				state;
	t_ui_window		*win;
	void			*parent;
	int				parent_type;
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
 * NOTE:
 * LEFT/RIGHT can be combined with TOP/BOT
*/
enum e_ui_text_align
{
	UI_TEXT_ALIGN_NONE = 0x00000000,
	UI_TEXT_ALIGN_LEFT = 0x00000001,
	UI_TEXT_ALIGN_RIGHT = 0x00000010,
	UI_TEXT_ALIGN_TOP = 0x00000100,
	UI_TEXT_ALIGN_BOT = 0x00001000,
	UI_TEXT_ALIGN_CENTER = 0x00010000,
};
/*
 * bool	font_recreate;		set this to 1 if you edit something of the font like.. size. will be set to 0 after creation.
 * bool	texture_recreate;	set this to 1 if you edit something of the texture like.. anything.. will be set to 0 after creation.
 * t_vec2i	text_wh;		w and h of the whole text.
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
}						t_ui_slider;

/*
 * Basically a button that is toggle:able
*/
typedef struct s_ui_checkbox
{
	t_ui_element		*elem;
}						t_ui_checkbox;


/*
 * t_list		*recipes; list of t_ui_element_recipe /button/label/menu/window...
 * t_list		*element; list of t_ui_element /button/label/menu/...
 * t_list		*window; list of t_ui_window .
*/
typedef struct s_ui_layout
{
	t_list				*recipes;
	t_list				*elements;
	t_list				*windows;
}						t_ui_layout;

// Layout
void					ui_layout_event(t_ui_layout *layout, SDL_Event e);
int						ui_layout_render(t_ui_layout *layout);
t_ui_element			*ui_layout_get_element_by_id(t_ui_layout *layout, char *id);

// Vec
void					print_vec(float *vec, size_t size);
void					print_veci(int *vec, size_t size);

// Window
void					ui_window_new(t_ui_window *win, char *title, t_vec4i pos);
void					ui_window_event(t_ui_window *win, SDL_Event e);
int						ui_window_render(t_ui_window *win);
void					ui_window_free(void *win);

// Element
void					ui_element_new(t_ui_window *win, t_ui_element *elem);
void					ui_element_textures_redo(t_ui_element *elem);
int						ui_element_render(t_ui_element *elem);
void					ui_element_pos_set(t_ui_element *elem, t_vec4i pos);
void					ui_element_pos_set2(t_ui_element *elem, t_vec2i pos);
void					ui_element_color_set(t_ui_element *elem, int state, Uint32 color);
void					ui_element_image_set(t_ui_element *elem, int state, SDL_Surface *image);
void					ui_element_image_set_from_path(t_ui_element *elem, int state, char *image_path);
void					ui_element_parent_set(t_ui_element *elem, t_ui_element *parent, int type, bool *show);

// Texture
SDL_Texture				*ui_texture_create_from_text_recipe(SDL_Renderer *renderer, t_ui_label *recipe);
SDL_Texture				*ui_create_texture(SDL_Renderer *renderer, t_vec4i pos);
void					ui_texture_draw_border(SDL_Renderer *renderer, SDL_Texture *texture, size_t thicc, Uint32 color);
void					ui_texture_fill(SDL_Renderer *renderer, SDL_Texture *texture, Uint32 color);
void					ui_texture_fill_rect(SDL_Renderer *renderer, SDL_Texture *texture, Uint32 color, t_vec4i rect);

// Surface
SDL_Surface				*ui_create_surface(t_vec4i size);

// Help
int						point_in_rect(t_vec2i point, t_vec4i rect);
t_rgba					hex_to_rgba(Uint32 color_hex);
Uint32					rgba_to_hex(t_rgba rgba);
char					*get_text_to_char_at_x(char *str, int x, TTF_Font *font);
int						get_x_of_char_in_text(char *str, int nth_char, TTF_Font *font);
int						get_nth_char_of_text_at_x(char *str, int x, TTF_Font *font);

// Label
void					ui_label_new(t_ui_window *win, t_ui_element *label);
void					ui_label_texture_redo(t_ui_element *label);
void					ui_label_event(t_ui_element *elem, SDL_Event e);
int						ui_label_render(t_ui_element *label);
// edit
void					ui_label_text_set(t_ui_element *label, char *text);
void					ui_label_font_set(t_ui_element *label, char *font_path);
void					ui_label_size_set(t_ui_element *label, size_t size);
void					ui_label_color_set(t_ui_element *label, Uint32 color);
void					ui_label_text_center(t_ui_element *elem);
void					ui_label_text_align(t_ui_element *elem, int align);
// end edit
void					ui_label_free(void *label);

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

// Load
void					ui_print_accepted(void);
void					ui_load(t_ui_layout *layout, char *ui_file_path);

#endif
