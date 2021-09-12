#include "libui.h"

typedef struct s_ui_layout_v2	t_ui_layout_v2;

struct s_ui_layout_v2
{
	char			*layout_file;
	char			*style_file;
};

///////////////
// Layout
///////////////
void				ui_layout_load_v2(t_ui_layout_v2 *layout, char *file);
