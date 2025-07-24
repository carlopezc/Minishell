/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_asterisk.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 20:06:41 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/24 03:24:24 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_init_int(int (*i)[3])
{
	(*i)[0] = 0;
	(*i)[1] = 0;
	(*i)[2] = 0;
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
	if (!s_input)
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
	t_quote	quote;

	i = 0;
	final_input = NULL;
	ft_init_quote(&quote);
	if (!*input)
		return (1);
	while ((*input)[i])
	{
		if (!i || (*input)[i - 1] != '\\')
			ft_check_quote(&quote, (*input)[i]);
		if ((*input)[i] == '*' && !quote.flag)
		{
			final_input = ft_strjoin_char(final_input, (*input)[i++]);
			while ((*input)[i] && (*input)[i] == '*')
				i++;
		}
		else
			final_input = ft_strjoin_char(final_input, (*input)[i++]);
	}
	ft_safe_free((void **)input);
	*input = final_input;
	return (1);
}

int	ft_find_asterisk(char *input)
{
	char	*asterisk;
	char	*space;

	asterisk = ft_strchr(input, '*');
	if (asterisk && *(asterisk - 1) && (*(asterisk - 1) == '\\'))
		asterisk = ft_strchr(asterisk + 1, '*');
	space = ft_strchr(input, ' ');
	if (*input == '*' && (input + 1)
		&& (*(input + 1) == '\"' || *(input + 1) == '\''))
		return (0);
	if (!space && !asterisk)
		return (0);
	if (!space && asterisk)
		return (1);
	if (space && !asterisk)
		return (0);
	if (asterisk < space)
		return (1);
	return (0);
}
