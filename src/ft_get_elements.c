/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_elements.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:36:42 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/26 04:03:42 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

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
	char	flag;
	int		i;
	
	tmp = NULL;
	flag = -1;
	i = 0;
	if ((value[i] == '\'') || value[i] == '\"')
	{
		flag = value[i];
		i++;
	}
	while (value[i] && value[i + 1])
	{
		if ((value[i] == '\'' || value[i] == '\"') && (!i || value[i - 1] != '\\')
				&& value[i] == flag)
			i++;
		else
		{
			tmp = ft_strjoin_char(tmp, value[i++]);
			if (!tmp)
				return (NULL);
		}
	}
	if (flag == value[i])
		i++;
	else
	{
		tmp = ft_strjoin_char(tmp, value[i++]);
		if (!tmp)
			return (NULL);
	}
	ft_safe_free((void **)&value);
	return (tmp);
}

int	ft_get_value_join(char **value, char *equal, int *i, char **tmp)
{
	*tmp = ft_strjoin_char(*value, equal[(*i)++]);
	if (!*tmp)
	{
		ft_safe_free((void **)value);
		return (0);
	}
	*value = *tmp;
	return (1);
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
			if (ft_get_value_join(&value, equal, &i, &tmp) == 0)
				return (NULL);
		}
	}
	return (ft_clean_value(value));
}

char	*ft_get_name(char *str)
{
	int		i;
	int		init;
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
