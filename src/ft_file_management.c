/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_file_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 20:03:11 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/24 02:47:32 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_create_element_dup(char ***new, int size)
{
	*new = (char **)malloc(size * sizeof(char *));
	if (!*new)
		return (0);
	(*new)[size - 1] = NULL;
	return (1);
}

int	ft_delete_item(char ***elements, char *delete)
{
	int		i;
	int		j;
	char	**new;
	int		size;

	i = -1;
	j = 0;
	if (!elements || !*elements)
		return (1);
	size = ft_arraylen(*elements);
	if (!ft_create_element_dup(&new, size))
		return (0);
	while ((*elements)[++i])
	{
		if ((*elements)[i] != delete)
		{
			new[j] = ft_strdup((*elements)[i]);
			if (!new[j++])
				return (ft_free_array(new), 0);
		}
	}
	ft_free_array(*elements);
	*elements = new;
	return (1);
}

int	ft_fill_files_loop(DIR **dir, struct dirent **entry, char ***files, int *i)
{
	while (*entry)
	{
		if ((*entry)->d_name[0] != '.')
		{
			(*files)[*i] = ft_strdup((*entry)->d_name);
			if (!(*files)[(*i)++])
					return (0);
		}
		(*entry) = readdir(*dir);
	}
	return (1);
}

char	**ft_fill_files(int size)
{
	char			**files;
	struct dirent	*entry;
	int				i;
	DIR				*dir;

	i = 0;
	dir = opendir(".");
	if (!dir)
		return (ft_printf("Error opening dir\n"), NULL);
	files = (char **)malloc((size + 1) * sizeof(char *));
	if (!files)
		return (NULL);
	entry = readdir(dir);
	files[size] = NULL;
	if (!ft_fill_files_loop(&dir, &entry, &files, &i))
			return (closedir(dir), ft_free_array(files), NULL);
	closedir(dir);
	return (files);
}

char	**ft_get_elements(void)
{
	char	**files;
	DIR		*dir;
	int		n_files;

	files = NULL;
	n_files = 0;
	dir = opendir(".");
	if (!dir)
		return (ft_printf("Error opening dir\n"), NULL);
	n_files = ft_count_files(dir);
	files = ft_fill_files(n_files);
	return (files);
}
