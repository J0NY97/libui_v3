#include "libui.h"

typedef struct s_ui_layout_v2	t_ui_layout_v2;

struct s_ui_layout_v2
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
};

typedef struct s_ui_family
{
	char			*parent_id;
	int				parent_type;
	char			**children_strings;
	t_list			*children; // t_list of t_ui_family;
}					t_ui_family;

typedef struct s_ui_recipe_v2
{
	char			*id;
	t_vec4			pos;
	Uint32			bg_colors[UI_STATE_AMOUNT];
	char			*title;
	Uint32			text_color;
	int				text_align;
	int				text_style;
	char			*font;

	bool			pos_set[VEC4_SIZE];
	bool			bg_colors_set[UI_STATE_AMOUNT];
	bool			text_color_set;
	bool			text_style_set;
}					t_ui_recipe_v2;

///////////////
// Layout
///////////////
void				ui_layout_event_v2(t_ui_layout_v2 *layout, SDL_Event e);
void				ui_layout_render_v2(t_ui_layout_v2 *layout);
void				ui_layout_load_v2(t_ui_layout_v2 *layout, char *file);
void				layout_apply_style(t_ui_layout_v2 *layout);
void				layout_make_recipes(t_ui_layout_v2 *layout);
void				layout_split_styles(t_ui_layout_v2 *layout);
void				layout_read_style(t_ui_layout_v2 *layout);
void				layout_compile_elements(t_ui_layout_v2 *layout);
void				layout_make_family_trees(t_ui_layout_v2 *layout);
void				layout_split_elements(t_ui_layout_v2 *layout);
void				layout_read_file(t_ui_layout_v2 *layout, char *file);
