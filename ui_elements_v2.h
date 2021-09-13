#include "libui.h"

typedef struct s_ui_layout_v2	t_ui_layout_v2;

struct s_ui_layout_v2
{
	char			*layout_file;
	char			*layout_file_content;
	char			**layout_element_strings;
	char			*style_file;
	t_list			*windows;
	t_list			*elements;

	t_list			*families;	// t_list of t_ui_family;
};

typedef struct s_ui_family
{
	char			*parent_id;
	int				parent_type;
	char			**children_strings;
	t_list			*children; // t_list of t_ui_family;
}					t_ui_family;

///////////////
// Layout
///////////////
void				ui_layout_load_v2(t_ui_layout_v2 *layout, char *file);
void				layout_apply_style(t_ui_layout_v2 *layout);
void				layout_read_style(t_ui_layout_v2 *layout);
void				layout_compile_elements(t_ui_layout_v2 *layout);
void				layout_make_family_trees(t_ui_layout_v2 *layout);
void				layout_split_elements(t_ui_layout_v2 *layout);
void				layout_read_file(t_ui_layout_v2 *layout, char *file);
