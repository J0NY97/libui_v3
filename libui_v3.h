#ifndef LIBUI_V3_H
# define LIBUI_V3_H
# define SDL_MAIN_HANDLED
# include "SDL.h"
# include "SDL_image.h"
# include "SDL_ttf.h"
# include "libft.h"
# include "libpf.h"
# include "stdio.h"
//# include "path.h"

# define VEC2_SIZE 2
# define VEC4_SIZE 4

typedef struct s_vec2i
{
	union {
		struct {
			int x;
			int y;
		};
		int v[VEC2_SIZE];
	};
}					t_vec2i;

typedef struct s_vec4
{
	union {
		struct {
			float x;
			float y;
			float w;
			float h;
		};
		float v[VEC4_SIZE];
	};
}					t_vec4;

typedef struct s_vec4i
{
	union {
		struct {
			int x;
			int y;
			int w;
			int h;
		};
		int v[VEC4_SIZE];
	};
}					t_vec4i;

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

enum	e_element_states
{
	UI_STATE_DEFAULT = 0,
	UI_STATE_HOVER,
	UI_STATE_CLICK,
	UI_STATE_AMOUNT
};

enum	e_element_types
{
	UI_TYPE_NONE = -1,
	UI_TYPE_ELEMENT = 0,
	UI_TYPE_LABEL,
	UI_TYPE_BUTTON,
	UI_TYPE_MENU,
	UI_TYPE_WINDOW,
	UI_TYPE_AMOUNT
};

/*
 * t_vec4i		pos;						the position of the elem relative to its parent.
 * t_vec4i		screen_pos;					the position of the elem relative to the screen.
 * SDL_Texture	*states[UI_STATE_AMOUNT];	textures for all the different states there are.
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
	t_ui_element		elem;
	t_list				*children;
}						t_ui_menu;

/*
 * bool	font_recreate;	set this to 1 if you edit something of the font like.. size. will be set to 0 after creation.
*/
typedef struct s_ui_text_recipe
{
	char				*text;
	char				*font_path;
	unsigned int		font_size;
	Uint32				font_color;
	int					max_w;
	TTF_Font			*font;
	bool				font_recreate;
	bool				texture_recreate;
}						t_ui_text_recipe;

typedef struct s_ui_label
{
	t_vec4i				pos;
	t_vec4i				screen_pos;
	void				*parent;
	int					parent_type;
	bool				*parent_show;
	t_ui_window			*win;
	SDL_Texture			*texture;
	t_ui_text_recipe	recipe;
	bool				show;
}						t_ui_label;

typedef struct s_ui_button
{
	t_ui_element		elem;
	t_ui_label			label;
}						t_ui_button;

// NOTE: this is just brainstorming, some of these should probably be in a create_element_from_recipe.
typedef struct s_ui_texture_recipe
{
	t_vec2i			size;
	float			opacity; // in css all child elements get same opacity.
	Uint32			bg_color;
	Uint32			*bg_image; // pixels
	t_vec4i			bg_image_pos;
	int				border_size;
	Uint32			border_color;
	// Text
	char			*text;
	char			*font_path;
	int				font_size;
	Uint32			font_color;
	TTF_Font		*font;
}					t_ui_texture_recipe;

// Vec
void				print_vec(float *vec, size_t size);

// Window
void				ui_window_new(t_ui_window *win, char *title, t_vec4i pos);
void				ui_window_event(t_ui_window *win, SDL_Event e);
void				ui_window_render(t_ui_window *win);
void				ui_window_get(void *win);
void				ui_window_free(void *win);

// Element
void				ui_element_new(t_ui_window *win, t_ui_element *elem);
void				ui_element_textures_redo(t_ui_element *elem);
int					ui_element_render(t_ui_element *elem);
void				ui_element_pos_set(t_ui_element *elem, t_vec4i pos);
void				ui_element_color_set(t_ui_element *elem, int state, Uint32 color);

// Texture
SDL_Texture			*ui_texture_create_from_text_recipe(SDL_Renderer *renderer, t_ui_text_recipe *recipe);
SDL_Texture			*ui_create_texture(SDL_Renderer *renderer, t_vec4i pos);
void				ui_texture_draw_border(SDL_Renderer *renderer, SDL_Texture *texture, size_t thicc, Uint32 color);
void				ui_texture_fill_rect(SDL_Renderer *renderer, SDL_Texture *texture, Uint32 color);

// Surface
SDL_Surface			*ui_create_surface(t_vec4i size);

// Help
t_vec4i				vec4i(int x, int y, int w, int h);
int					point_in_rect(t_vec2i point, t_vec4i rect);
t_rgba				hex_to_rgba(Uint32 color_hex);
Uint32				rgba_to_hex(t_rgba rgba);

// Label
void				ui_label_new(t_ui_window *win, t_ui_label *label);
void				ui_label_texture_redo(t_ui_label *label);
void				ui_label_render(t_ui_label *label);
// edit
void				ui_label_text_set(t_ui_label *label, char *text);
void				ui_label_pos_set(t_ui_label *label, t_vec4i pos);
void				ui_label_size_set(t_ui_label *label, size_t size);
void				ui_label_color_set(t_ui_label *label, Uint32 color);
// end edit
void				ui_label_get(void *label);
void				ui_label_free(void *label);

// Button
void				ui_button_new(t_ui_window *win, t_ui_button *button);
void				ui_button_event(t_ui_button *button, SDL_Event e);
bool				ui_button(t_ui_button *button);
void				ui_button_render(t_ui_button *button);
void				ui_button_get(void *button);
void				ui_button_free(void *button);

// Menu
void				ui_menu_new(t_ui_window *win, t_ui_menu *menu);
void				ui_menu_child_add(t_ui_menu *menu, void *child, int type);
void				ui_menu_render(t_ui_menu *menu);
void				ui_menu_event(t_ui_menu *menu, SDL_Event e);
void				ui_menu_get(void *menu);
void				ui_menu_free(void *menu);

// Load
void				ui_print_accepted(void);
void				ui_load(char *ui_file_path);

#endif
