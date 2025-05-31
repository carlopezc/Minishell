/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_file_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 20:03:11 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/30 20:05:03 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_delete_item(char ***elements, char *delete)
{
	int		i;
	int		j;
	char	**new;
	int		size;

	i = 0;
	j = 0;
	size = 0;
	if (!elements | !*elements)
		return (1);
	size = ft_arraylen(*elements);
	new = (char **)malloc(size * sizeof(char *));
	if (!new)
		return (0);
	new[size - 1] = NULL;
	while ((*elements)[i])
	{
		if ((*elements)[i] != delete)
		{
			new[j] = ft_strdup((*elements)[i]);
			if (!new[j])
				return (ft_free_array(new), 0);
			j++;
		}
		i++;
	}
	ft_free_array(*elements);
	*elements = new;
	return (1);
}

char	**ft_fill_files(int size)
{
	char			**files;
	struct dirent	*entry;
	DIR				*dir;
	int				i;

	dir = opendir(".");
	if (!dir)
		return (ft_printf("Error opening dir\n"), NULL);
	i = 0;
	files = (char **)malloc((size + 1) * sizeof(char *));
	if (!files)
		return (NULL);
	entry = readdir(dir);
	files[size] = NULL;
	while (entry)
	{
		if (entry->d_name[0] != '.')
		{
			files[i] = ft_strdup(entry->d_name);
			if (!files[i])
				return (ft_free_array(files), NULL);
			i++;
		}
		entry = readdir(dir);
	}
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
	closedir(dir);
	return (files);
}
