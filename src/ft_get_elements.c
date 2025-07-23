/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_elements.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:36:42 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/23 17:50:29 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_get_size(char *input, char c)
{
	int		i;
	int		size;

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

char	*ft_get_next(char *input, int *i)
{
	int	start;

	start = 0;
	while (input[*i] && input[*i] == ' ')
		(*i)++;
	start = *i;
	while (input[*i] && input[*i] != ' ' && input[*i] != '|'
		&& input[*i] != '<' && input[*i] != '>' && input[*i] != '&')
		(*i)++;
	return (ft_substr(input, start, *i - start));
}

static char	*ft_clean_value(char *value)
{
	char	*tmp;

	tmp = NULL;
	if ((value[0] == '\"' || value[0] == '\'')
		&& ((value[ft_strlen(value) - 1] == '\"')
			|| (value[ft_strlen(value) - 1] == '\'')))
	{
		tmp = ft_substr(value, 1, ft_strlen(value) - 2);
		ft_safe_free((void **)&value);
		value = tmp;
	}
	return (value);
}

char	*ft_get_value(char *str)
{
	int		i;
	char	*value;
	char	*equal;
	char	*tmp;

	i = 0;
	equal = ft_strchr(str, '=');
	if (!equal)
		return (NULL);
	else if (!*(equal + 1))
		value = ft_strdup("");
	else
	{
		i++;
		value = ft_strdup("");
		while (equal[i])
		{
			tmp = ft_strjoin_char(value, equal[i++]);
			if (!tmp)
			{
				ft_safe_free((void **)value);
				return (NULL);
			}
			value = tmp;
		}
	}
	return (ft_clean_value(value));
}

char	*ft_get_name(char *str)
{
	int		i;
	int	init;
	char	*name;

	i = 0;
	init = 0;
	if (!str)
		return (NULL);
	if (str[i] && (str[i] == '\'' || str[i] == '\"'))
	{
		init++;
		i++;
	}
	while (str && str[i])
	{
		if (str[i] == '\'' || str[i] == '\"' || str[i] == '=')
			break ;
		else if (str[i] == '+' && str[i + 1] && str[i + 1] == '=')
			break ;
		i++;
	}
	name = ft_substr(str, init, i - init);
	return (name);
}
