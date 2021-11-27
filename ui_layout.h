#include "libui.h"

typedef struct s_ui_layout	t_ui_layout;
typedef struct s_ui_element	t_ui_element;
typedef struct s_ui_window	t_ui_window;
typedef struct s_ui_recipe	t_ui_recipe;

struct s_ui_layout
{
	char			*layout_file;
	char			*layout_file_content;
	char			**layout_element_strings;
	char			*style_file;
	char			*style_file_content;
	char			**style_recipe_strings;
	t_list			*windows;
	t_list			*elements;

	t_list			*families;	// t_list of t_ui_family;
	t_list			*recipes; // t_list of t_ui_recipe_v2;

	char			**resource_dirs;
};

typedef struct s_ui_family
{
	char			*parent_id;
	int				parent_type;
	char			**children_strings;
	t_list			*children; // t_list of t_ui_family;
}					t_ui_family;

/*
 * int		value[3];		0 : value, 1 : min, 2 : max;
 * Uint32	input_type;		only numbers, only letters, only everything (default)
*/
typedef struct s_ui_recipe
{
	char			*id;
	t_vec4			pos;
	Uint32			bg_colors[UI_STATE_AMOUNT];
	char			*bg_images[UI_STATE_AMOUNT];
	int				z;
	bool			show;

	bool			pos_set[VEC4_SIZE];
	bool			bg_colors_set[UI_STATE_AMOUNT];
	bool			bg_images_set[UI_STATE_AMOUNT];
	bool			z_set;
	bool			show_set;

	// Label only
	char			*title;
	Uint32			text_color;
	int				text_size;
	int				text_align;
	int				text_style;
	char			*font;
	t_vec4			text_pos;
	bool			remove_title;

	bool			text_size_set;
	bool			text_color_set;
	bool			text_style_set;
	bool			text_align_set;
	bool			text_pos_set[VEC4_SIZE];

	// Slider only
	int				value[3];
	int				value_set[3];

	// Window / Menu only
	char			**flags;

	// Input only
	bool			input_type_set;
	Uint32			input_type;
	char			*placeholder_text;

	// Scrollbar only
	char			*target;
}					t_ui_recipe;

///////////////
// Layout
///////////////
void				ui_layout_event(t_ui_layout *layout, SDL_Event e);
void				ui_layout_render(t_ui_layout *layout);
void				ui_layout_list_render(t_list *list);
void				ui_layout_load(t_ui_layout *layout, char *file);
void				layout_apply_style(t_ui_layout *layout);
void				layout_make_recipes(t_ui_layout *layout);
void				layout_split_styles(t_ui_layout *layout);
void				layout_read_style(t_ui_layout *layout);
void				layout_compile_elements(t_ui_layout *layout);
void				layout_make_family_trees(t_ui_layout *layout);
void				layout_split_elements(t_ui_layout *layout);
void				layout_read_file(t_ui_layout *layout, char *file);
void				print_recipe(t_ui_recipe *recipe);
t_ui_window			*ui_layout_get_window(t_ui_layout *layout, char *id);
t_ui_element		*ui_layout_get_element(t_ui_layout *layout, char *id);
t_ui_recipe			*ui_layout_get_recipe(t_ui_layout *layout, char *id);

///////////////
// Recipe
///////////////
void				ui_recipe_free(t_ui_recipe *recipe);
