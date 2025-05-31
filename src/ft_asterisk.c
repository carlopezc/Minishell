/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_asterisk.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 20:06:41 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/30 21:26:19 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_check_asterisk(char *input, char ***elements)
{
	char	**s_input;
	int		i;
	int		j;
	int		q;
	int		flag;

	i = 0;
	j = 0;
	q = 0;
	flag = 0;
	s_input = ft_split_asterisk(input, '*');
	if (!s_input || !*s_input)
		return (0);
	while ((*elements)[j])
	{
		while (s_input[i])
		{
			//caso en que solo hay un asterisco
			if (!ft_strncmp(s_input[i], "*", 2))
				i++;
			else if (ft_strchr(s_input[i], '*'))
			{
				while ((*elements)[j][q])
				{
					if (i == 0)
					{
						if (!ft_strncmp(&(*elements)[j][0],
										s_input[i], ft_strlen(s_input[i]) - 1))
						{
							q = q + ft_strlen(s_input[i]) - 1;
							flag = 1;
							break ;
						}
						flag = -1;
						break ;
						//primero, tiene que empezar con las letras
					}
					if (!ft_strncmp(&(*elements)[j][q],
										s_input[i], ft_strlen(s_input[i]) - 1))
					{
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
		i = 0;
		q = 0;
		if (flag != 1)
			j++;
		flag = 0;
	}
	return (1);
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
	int		i;
	int		j;
	int		size;
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

int	ft_parse_asterisk(char **input)
{
	int		i;
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

int	ft_find_asterisk(char *input)
{
	char	*asterisk;
	char	*space;

	asterisk = ft_strchr(input, '*');
	space = ft_strchr(input, ' ');
	if (*input == '*' && (input + 1)
		&& (*(input + 1) == '\"' || *(input + 1) == '\''))
		return (0);
	if (!space && asterisk)
		return (1);
	if (space && !asterisk)
		return (0);
	if (asterisk < space)
		return (1);
	return (0);
}
