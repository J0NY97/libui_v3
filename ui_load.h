#ifndef UI_LOAD_H
# define UI_LOAD_H
# include "ui_enum.h"
# include "ui_vec.h"
# include "libui_v3.h"

typedef struct s_ui_window	t_ui_window;
/*
 * int		type;		one of enum e_element_type
*/
typedef struct s_ui_recipe
{
	char				*id;
	int					type;

	t_vec4i				pos;	
	bool				pos_set;

	char				*title;

	char				*font_path;
	bool				font_path_set;

	Uint32				font_size;
	bool				font_size_set;

	Uint32				font_color;
	bool				font_color_set;

	Uint32				bg_color[UI_STATE_AMOUNT];
	bool				bg_color_set;

	char				*bg_image[UI_STATE_AMOUNT];
	bool				bg_image_set;

	char				**children_ids;
	int					child_amount;
	t_list				*recipes;
}						t_ui_recipe;

typedef struct s_ui_key_value
{
	char		*key;
	char		*value;
}				t_ui_key_value;

typedef struct	s_ui_get
{
	int				*len; // len of kv
	t_ui_key_value	*kv;
	t_ui_recipe		*recipe;
}					t_ui_get;

void	ui_layout_add_child(t_list **list, t_list *recipes, t_ui_window *win, t_ui_recipe *recipe);

#endif
