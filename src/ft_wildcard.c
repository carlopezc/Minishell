/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcard.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:57:16 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/28 12:56:08 by carlopez         ###   ########.fr       */
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

int	ft_get_size(char *input, char c)
{
	int 	i;
	int	size;

	i = 0;
	size = 0;
	while (input[i])
	{
		if (input[i] == c)
			size++;
		i++;
	}
	if (input[i - 1] != c)
		size++;
	return (size);
}

int	ft_fill_word(char *input, int *i, char **s_input, char c)
{
	int	start;

	start = *i;
	while (input[*i])
	{
		if (input[*i] == c)
		{
			(*i)++;
			*s_input = ft_substr(input, start, *i - start);
			if (!*s_input)
				return (0);
			return (1);
		}
		(*i)++;
	}
	if (!input[*i] && input[*i - 1] != '*')
		*s_input = ft_substr(input, start, *i - start);
	if (!*s_input)
		return (0);
	return (1);
}

char	**ft_split_asterisk(char *input, char c)
{
	int	i;
	int	j;
	int	size;
	char	**s_input;

	i = 0;
	j = 0;
	size = ft_get_size(input, c);
	s_input = (char **)malloc((size + 1) * sizeof(char *));
	if (!s_input || !*s_input)
		return (NULL);
	s_input[size] = NULL;
	while (i < size)
	{
		if (!ft_fill_word(input, &j, &s_input[i], c))
			return (ft_free_array(s_input), NULL);
		i++;
	}
	return (s_input);
}

int	ft_check_asterisk(char *input, char ***elements)
{
	char	**s_input;
	int	i;
	int	j;
	int	q;
	int	flag;

	i = 0;
	j = 0;
	q = 0;
	flag = 0;
	ft_printf("Entra en check asterisk\n");
	s_input = ft_split_asterisk(input, '*');
	if (!s_input || !*s_input)
		return (0);
	while (s_input[i])
		ft_printf("%s\n", s_input[i++]);
	i = 0;
	ft_printf("Entra ya en el bucle para comprobar\n");
	while ((*elements)[j])
	{
		ft_printf("A comprobar: %s\n", (*elements)[j]);
		while (s_input[i])
		{
			//caso en que solo hay un asterisco
			ft_printf("Con: %s\n", s_input[i]);
			if (!ft_strncmp(s_input[i], "*", 2))
			{
				ft_printf("Entra en asterisco solo\n");
				i++;
			}
			else if (ft_strchr(s_input[i], '*'))
			{
				ft_printf("Entra en asterisco y letras\n");
				while ((*elements)[j][q])
				{
					if (i == 0)
					{
						ft_printf("Entra en i = 0\n");
						if (!ft_strncmp(&(*elements)[j][0], s_input[i], ft_strlen(s_input[i]) - 1))
						{
							ft_printf("Encuentra coincidencia\n");
							q = q + ft_strlen(s_input[i]) - 1;
							flag = 1;
							break ;
						}
						flag = -1;
						ft_printf("Sale\n");
						break ;
						//primero, tiene que empezar con las letras
					}
					if (!ft_strncmp(&(*elements)[j][q], s_input[i], ft_strlen(s_input[i]) - 1))
					{
						ft_printf("Encuentra coincidencia\n");
						q = q + ft_strlen(s_input[i]) - 1;
						flag = 1;
						break ;
						//menos uno para que no pille el asterisco
					}
					q++;
					//si no, que lo encuentre despues de la i que lleve
				}
				if ((!((*elements)[j][q]) && !flag) || flag == -1)
				{	
					ft_printf("Borra\n");
					if (!ft_delete_item(elements, (*elements)[j]))
						return (0);
					flag = 1;
					//lo elimina y no suma
					break ;
				}
				else
					i++;
				//si no hay coincidencia que lo borre y no sume,
				//si la hay que sume
				//letras + * final
			}
			else
			{
				ft_printf("Entra en solo letras\n");
				while ((*elements)[j][q])
				{
					if (!ft_strncmp(&(*elements)[j][q], s_input[i], ft_strlen(s_input[i]) + 1))
					{
						flag = 1;
						break ;
					}
					q++;
				}
				if (!(*elements)[j][q] && !flag)
				{
					ft_printf("Borra\n");
					if (!ft_delete_item(elements, (*elements)[j]))
						return (0);
					flag = 1;
					break ;
					//lo elimina y no suma
				}
				else
					i++;	
				//caso solo letras
			}
			flag = 0;
		}
		ft_printf("Sale del bucle s_input\n");
		i = 0;
		q = 0;
		if (flag != 1)
			j++;
		flag = 0;
	}
	return (1);
}

int	ft_parse_asterisk(char **input)
{
	int	i;
	char	*final_input;

	i = 0;
	final_input = NULL;
	if (!*input)
		return (1);
	while ((*input)[i])
	{
		if ((*input)[i] == '*')
		{
			final_input = ft_strjoin_char(final_input, (*input)[i++]);
			while ((*input)[i] && (*input)[i] == '*')
				i++;
		}
		else
			final_input = ft_strjoin_char(final_input, (*input)[i++]);
	}
	final_input = ft_strjoin_char(final_input, '\0');
	*input = final_input;
	return (1);
}

char	*ft_expand_wildcard(char *input)
{
	char	**elements;

	ft_printf("ENTRAMOS A EXPANDIR WILDCARD \n");
	ft_printf("EL input es %s\n", input);
	elements = ft_get_elements();
	if (!elements)
		return (ft_free_array(elements), NULL);
	/*
	if (!ft_parse_asterisk(&input))
		return (ft_free_array(elements), NULL);
		*/
	if (!ft_strncmp(input, "*", 2))
		return (ft_create_array(elements));
	if (!ft_check_asterisk(input, &elements))
		return (NULL);
	return (ft_create_array(elements));
}

int	ft_find_asterisk(char *input)
{
	char	*asterisk;
	char	*space;

	asterisk = ft_strchr(input, '*');
	space = ft_strchr(input, ' ');
	if (*input == '*' && (input + 1) && (*(input + 1) == '\"' || *(input + 1) == '\''))
		return (0);
	if (!space && asterisk)
		return (1);
	if (space && !asterisk)
		return (0);
	if (asterisk < space)
		return (1);
	return (0);
}

int	ft_check_wildcard(t_token **tokens)
{
	int	i;
	t_token	*tmp;
	char	*str;
	t_quote	quote;
	char	*expanded;
	char	*str_final;
	char	*sub;
	int	start;

	i = 0;
	quote.type = 0;
	quote.flag = 0;
	tmp = *tokens;
	start = 0;
	sub = NULL;
	str_final = NULL;
	while (tmp)
	{
		str = tmp->str;
		if (!ft_parse_asterisk(&str))
			return (0);
		while (str[i])
		{
			if (!str[i - 1] || str[i - 1] != '\\')
				ft_check_quote(&quote, str[i], &i);
			if (str[i] == '*' && !quote.flag && (!str[i - 1] || str[i - 1] != '\\'))
			{
				start = i;
				while (str[start - 1] && str[start - 1] != ' ' && start > 0)
					start--;
				i++;
				while (str[i] && str[i] != ' ' && (!str[i - 1] || str[i] != '\\'))
					i++;
				ft_printf("i es : %d\n", i);
				sub = ft_substr(str, start, i - start);
				ft_printf("sub es %s\n", sub);
				expanded = ft_expand_wildcard(sub);
				ft_printf("Expanded es : %s\n", expanded);
				str_final = ft_strjoin(str_final, expanded);
				ft_printf("str_final es : %s\n", str_final);
				free(sub);
				free(expanded);
			}
			if (str[i] && (str[i] != '\'' && str[i] != '\"') && !ft_find_asterisk(&str[i]))
				str_final = ft_strjoin_char(str_final, str[i]);
			i++;
		}
		i = 0;
		tmp->str = ft_strdup(str_final);
		free(str_final);
		tmp = tmp->next;
	}
	return (1);
}
