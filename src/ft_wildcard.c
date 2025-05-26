/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcard.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:57:16 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/26 22:49:05 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_count_files(DIR *dir)
{
	struct dirent	*entry;
	int	files;

	files = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.')
			files++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (files);
}

char	**ft_fill_files(int size)
{
	char **files;
	struct	dirent *entry;
	DIR	*dir;
	int	i;

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
	DIR	*dir;
	int	n_files;

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

void	ft_print_elements(char **elements)
{
	int	i;

	i = 0;
	while (elements[i])
		ft_printf("%s ", elements[i++]);
	return ;
}

int	ft_delete_item(char ***elements, char *delete)
{
	int	i;
	int	j;
	char	**new;
	int	size;

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
			//puede que pete no se si strdup hace malloc
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

int	ft_compare_elements(char *str, int flag, char ***elements)
{
	int	i;
	int	check;
	int	j;
	int	equal;

	i = 0;
	check = 0;
	j = 0;
	equal = 0;
	ft_printf("flag es %d\n", flag);
	ft_printf("str es %s\n", str);
	while ((*elements)[i])	
	{
		if (!flag)
		{
			if (ft_strncmp((*elements)[i], str, ft_strlen(str)))
			{
				if (!ft_delete_item(elements, (*elements)[i]))
					return (0);
				check = 1;
			}
		}
		else
		{
			//caso de entre asteriscos
			while ((*elements)[i][j])
			{
				if (flag == 1 && !ft_strncmp(&(*elements)[i][j], str, ft_strlen(str)))
				{
					equal = 1;
					break ;
				}
				else if (flag == 2 && !ft_strncmp(&(*elements)[i][j], str, ft_strlen(str) + 1))
				{
					ft_printf("sale\n");
					equal = 1;
					break ;
				}
				j++;
			}
			if (!(*elements)[i][j] && !equal)
			{
				if (!ft_delete_item(elements, (*elements)[i]))
					return (0);
				check = 1;
			}
			equal = 0;
			j = 0;
		}
		if (!check)
			i++;
		check = 0;
	}
	return (1);
}

int	ft_check_asterisk(char *input, int *i, int flag, char ***elements)
{
	char	*str;
	int	start;

	start = 0;
	str = NULL;
	//if (*i == 0)
	//	return (1);
	if (!flag)
	{
		ft_printf("entra en no hay flag\n");
		start = *i;
		while (input[start] && input[start] != ' ' && start > 0)
			start--;
		str = ft_substr(input, start, *i);
		ft_printf("str es: %s\n", str);
		//(*i)++;
	}
	else
	{
		start = *i;
		ft_printf("start es %d\ni es %d\n", start, *i);
		if (!input[start])
		{
			start--;
			flag = 2;
		}
		if (input[start] == '*' && start > 0)
			start--;
		while (input[start] && input[start] != '*' && start > 0)
			start--;
		if (input[start] == '*')
			start++;
		ft_printf("start es %d\ni es %d\n", start, *i);
		str = ft_substr(input, start, *i - start);
		ft_printf("str %s\n", str);
	}
	if (!ft_compare_elements(str, flag, elements))
		return (0);
	return (1);
}

int	ft_parse_asterisk(char **input)
{
	int	i;
	int	len;
	int	j;
	char	*final_input;

	len = 0;
	i = 0;
	j = 0;
	final_input = NULL;
	if (!*input)
		return (1);
	while ((*input)[i])
	{
		if ((*input)[i] == '*')
		{
			len++;
			i++;
			while ((*input)[i] == '*')
				i++;
		}
		else
		{
			len++;
			i++;
		}
	}
	final_input = (char *)malloc((len + 1) * sizeof(char *));
	if (!final_input)
		return (0);
	i = 0;
	while ((*input)[i])
	{
		if ((*input)[i] == '*')
		{
			final_input[j++] = (*input)[i++];
			while ((*input)[i] && (*input)[i] == '*')
				i++;
		}
		else
			final_input[j++] = (*input)[i++];
	}
	final_input[j] = '\0';
	*input = final_input;
	return (1);
}

char	*ft_expand_wildcard(char *input)
{
	char	**elements;
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	elements = ft_get_elements();
	if (!elements)
		return (ft_free_array(elements), NULL);
	if (!ft_parse_asterisk(&input))
		return (ft_free_array(elements), NULL);
	if (!ft_strncmp(input, "*", 2))
		return (ft_create_array(elements));
	while (input[i])
	{
		if (input[i] == '*')
		{
			ft_check_asterisk(input, &i, flag, &elements);
			flag = 1;
		}
		i++;
	}
	if (!input[i] && input[i - 1] && input[i - 1] != '*')
		ft_check_asterisk(input, &i, flag, &elements);
	ft_print_elements(elements);
	return (ft_create_array(elements));
}

int	ft_check_wildcard(char **input)
{
	int	i;
	char	**s_input;
	char	*asterisk;

	i = 0;
	s_input = ft_split(*input, ' ');
	if (!s_input || !*s_input)
		return (0);
	while (s_input[i])
	{
		asterisk = ft_strchr(s_input[i], '*');
		if (asterisk && (!*(asterisk - 1) || *(asterisk - 1) != '\\'))
			s_input[i] = ft_expand_wildcard(s_input[i]); 
		if (!s_input[i])
			return (0);
		i++;
	}
	*input = ft_create_array(s_input);
	if (!*input)
		return (0);
	return (1);
}
