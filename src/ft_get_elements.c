/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_elements.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:36:42 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/26 03:15:43 by lbellmas         ###   ########.fr       */
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

static char	*ft_init(char *flag, int *i)
{
	*i = 0;
	*flag = 0;
	return (NULL);
}

static int	ft_clean2(char *value, int *i, char flag, char **tmp)
{
	if ((value[*i] == '\'' || value[*i] == '\"')
		&& (!*i || value[(*i) - 1] != '\\')
		&& value[*i] == flag)
		(*i)++;
	else
	{
		*tmp = ft_strjoin_char(*tmp, value[(*i)++]);
		if (!*tmp)
			return (0);
	}
	return (1);
}

char	*ft_clean_value(char *value)
{
	char	*tmp;
	char	flag;
	int		i;

	tmp = ft_init(&flag, &i);
	if ((value[i] == '\'') || value[i] == '\"')
	{
		flag = value[i];
		i++;
	}
	while (value[i] && value[i + 1])
	{
		if (!ft_clean2(value, &i, flag, &tmp))
			return (NULL);
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
		ft_safe_free((void **)(value));
		return (0);
	}
	*value = *tmp;
	return (1);
}
