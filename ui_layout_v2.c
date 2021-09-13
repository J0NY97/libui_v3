#include "libui.h"

void	ui_layout_load_v2(t_ui_layout_v2 *layout, char *file)
{
	layout_read_file(layout, file);
	layout_split_elements(layout);
	layout_compile_elements(layout);
	layout_read_style(layout);
	layout_apply_style(layout);
}

char	*get_file_content(char *file)
{
	char	*content;
	char	*line;
	char	*trim;
	size_t	len;
	ssize_t	nread;
	FILE	*fd;

	fd = fopen(file, "r");
	if (!fd)
		return (NULL);
	line = NULL;
	content = NULL;
	while (1)
	{
		nread = getline(&line, &len, fd);
		if (nread == -1)
			break ;
		trim = ft_supertrim(line);
		if (trim != NULL) // whole line was trimmed if is NULL (could happen if the line was a comment);
		{
			ft_stradd(&content, trim);
			ft_strdel(&trim);
		}
		ft_strdel(&line);
	}
	ft_printf("[%s] Content read.\n", __FUNCTION__);
	return (content);
}

void	layout_read_file(t_ui_layout_v2 *layout, char *file)
{
	memset(layout, 0, sizeof(t_ui_layout_v2));
	layout->layout_file = ft_strdup(file);
	layout->layout_file_content = get_file_content(file);
	ft_printf("%s", layout->layout_file_content);
}

char	**split_string_into_array(char *str)
{
	char	**final;
	int	i;
	int	j;
	int	opening;
	int	closing;
	int	prev_i;
	int	elem_count;

	i = -1;
	j = -1;
	opening = 0;
	closing = 0;
	prev_i = 0;
	elem_count = 0;
	final = malloc(sizeof(char *) * 1);
	while (str[++i])
	{
		if (str[i] == ';' && opening == closing)
		{
			final = realloc(final, sizeof(char *) * ++elem_count);
			final[elem_count - 1] = ft_strsub(str, prev_i, i - prev_i + 1);
			prev_i = i + 1;
		}
		else if (str[i] == '{')
			opening++;
		else if (str[i] == '}')
			closing++;
	}
	final = realloc(final, sizeof(char *) * ++elem_count);
	final[elem_count - 1] = 0;
	return (final);
}
/*
 * This splits everything from global scope separated with ';', to separeate strings;
*/
void	layout_split_elements(t_ui_layout_v2 *layout)
{
	int	k;

	layout->layout_element_strings = split_string_into_array(layout->layout_file_content);
	ft_printf("[%s] Done splitting elems.\n", __FUNCTION__);
	k = -1;
	while (layout->layout_element_strings[++k])
		ft_printf("#%d : \n%s\n", k, layout->layout_element_strings[k]);
}

/*
 * Returns a list of families;
*/
t_ui_family	*make_family_from_string(char *str)
{
	t_list		*families;
	t_ui_family	*family;
	t_ui_family	*child;
	char		**temp;
	char		**type_n_name; // dont free this, you can just set them inside the family things;
	int			len;
	int			rm_amount;
	char		*children;

	temp = ft_strsplitfirstoccurenceor(str, '{', ';');
	len = ft_strlen(temp[1]);
	if (len == 0) // probably some special stuff (like style : "path")
	{
		ft_arraydel(temp);
		return (NULL);
	}
	family = ft_memalloc(sizeof(t_ui_family));
	rm_amount = 0;
	if (temp[1][len - 1] == ';') // remove '}' and/or ';';
	{
		rm_amount++;
		if (temp[1][len - 2] == '}')
			rm_amount++;
		children = ft_strndup(temp[1], len - rm_amount);
		ft_printf("%s\n", children);
	}
	type_n_name = ft_strsplit(temp[0], ' ');
	ft_printf("%s %s\n{\n%s\n}\n", type_n_name[0], type_n_name[1], children);
	family->parent_id = type_n_name[1];
	family->parent_type = ui_element_type_from_string(type_n_name[0]);
	family->children_strings = split_string_into_array(children);
	int k = -1;
	while (family->children_strings[++k])
	{
		child = make_family_from_string(family->children_strings[k]);
		if (!child)
			break ;
		add_to_list(&family->children, child, sizeof(t_ui_family));
	}
	ft_arraydel(temp);
	return (family);
}

void	print_family(t_ui_family *family, int nth)
{
	t_list	*curr;
	int tabs;

	tabs = -1;
	while (++tabs < nth)
		ft_putchar('\t');
	ft_printf("%s\n", family->parent_id);
	tabs = -1;
	while (++tabs < nth)
		ft_putchar('\t');
	ft_putstr("{\n");
	curr = family->children;
	while (curr)
	{
		print_family(curr->content, nth + 1);
		curr = curr->next;
	}
	tabs = -1;
	while (++tabs < nth)
		ft_putchar('\t');
	ft_putstr("}\n");
}

/*
 * Takes the layout_file_content and makes the elements with their children.
 * This find window and creates its elements.
*/
void	layout_compile_elements(t_ui_layout_v2 *layout)
{
	int			i;
	char		**arr;
	t_ui_family	*family;

	arr = layout->layout_element_strings;
	if (!arr)
		return ;
	i = -1;
	while (arr[++i])
	{
		family = make_family_from_string(arr[i]);
		if (family)
			add_to_list(&layout->families, family, sizeof(t_ui_family));
	}
	// printing to make sure that the family is setup correctly;
	ft_printf("[%s] Print Families\n", __FUNCTION__);
	print_family(layout->families->content, 0);
}

void	layout_read_style(t_ui_layout_v2 *layout)
{
	
}

void	layout_apply_style(t_ui_layout_v2 *layout)
{
	
}
