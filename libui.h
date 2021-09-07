#ifndef LIBUI_V3_H
# define LIBUI_V3_H
# define SDL_MAIN_HANDLED
# include "SDL.h"
# include "SDL_image.h"
# include "SDL_ttf.h"
# include "ui_path.h"
# include "libft.h"
# include "libpf.h"
# include "stdio.h"
# include "ui_enum.h"
# include "ui_vec.h"
# include "ui_load.h"
# include "ui_elements.h"
# include "math.h"
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
t_ui_window				*ui_layout_get_window_by_id(t_ui_layout *layout, char *id);

// Vec
void					print_vec(float *vec, size_t size);
void					print_veci(int *vec, size_t size);

// Texture
SDL_Texture				*ui_texture_create_from_text_recipe(SDL_Renderer *renderer, t_ui_label *recipe);
SDL_Texture				*ui_create_texture(SDL_Renderer *renderer, t_vec2i pos);
void					ui_texture_draw_border(SDL_Renderer *renderer, SDL_Texture *texture, size_t thicc, Uint32 color);
void					ui_texture_fill(SDL_Renderer *renderer, SDL_Texture *texture, Uint32 color);
void					ui_texture_fill_rect(SDL_Renderer *renderer, SDL_Texture *texture, Uint32 color, t_vec4i rect);

// Surface
SDL_Surface				*ui_surface_new(int w, int h);
void					ui_surface_print(SDL_Surface *surface);
void					ui_surface_pixel_set(SDL_Surface *surface, int x, int y, Uint32 color);
void					ui_surface_line_draw_dot(SDL_Surface *surface, t_vec2i p1, t_vec2i p2, Uint32 color);
void					ui_surface_line_draw(SDL_Surface *surface, t_vec2i p1, t_vec2i p2, Uint32 color); // reduced
void					ui_surface_line_draw_orig(SDL_Surface *surface, t_vec2i p1, t_vec2i p2, Uint32 color); // orig
void					ui_surface_line_draw_nik(SDL_Surface *surf, t_vec2i v1, t_vec2i v2, Uint32 color); // nik
void					ui_surface_rect_draw(SDL_Surface *surface, t_vec2i p1, t_vec2i p2, Uint32 color);
void					ui_surface_circle_draw(SDL_Surface *surface, t_vec2i orig, int r, Uint32 color);
void					ui_surface_circle_draw_filled(SDL_Surface *surface, t_vec2i orig, int r, Uint32 color);

// Help
int						point_in_rect(t_vec2i point, t_vec4i rect);
t_rgba					hex_to_rgba(Uint32 color_hex);
Uint32					rgba_to_hex(t_rgba rgba);
Uint32					ui_color_come_up_with_name(Uint32 orig_col, float procentage);
char					*get_text_to_char_at_x(char *str, int x, TTF_Font *font);
int						get_x_of_char_in_text(char *str, int nth_char, TTF_Font *font);
int						get_nth_char_of_text_at_x(char *str, int x, TTF_Font *font);
t_ui_element			*ui_list_get_element_by_id(t_list *list, char *id);
int						dist(t_vec2i p1, t_vec2i p2);
float					fdist(t_vec2 p1, t_vec2 p2);

// Load
void					ui_print_accepted(void);
int						ui_layout_load(t_ui_layout *layout, char *ui_file_path);
t_ui_recipe				*ui_layout_get_recipe_by_id(t_ui_layout *layout, char *id);
t_ui_element			*ui_element_create_from_recipe(t_ui_window *win, t_ui_recipe *recipe, t_ui_layout *layout);

// Other
void					ui_sdl_init(void);

#endif
