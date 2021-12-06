#include "libui.h"

void	ui_recipe_free(void *recipe, size_t size)
{
	t_ui_recipe	*rcp;
	int			iii;

	rcp = recipe;
	if (!rcp)
		return ;
	ft_strdel(&rcp->id);
	iii = -1;
	while (++iii < UI_STATE_AMOUNT)
		if (rcp->bg_images_set[iii] && rcp->bg_images[iii])
			ft_strdel(&rcp->bg_images[iii]);
	ft_strdel(&rcp->title);
	ft_strdel(&rcp->font);
	ft_arraydel(rcp->flags);
	ft_strdel(&rcp->placeholder_text);
	ft_strdel(&rcp->target);
	(void)size;
}
