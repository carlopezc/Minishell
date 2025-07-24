/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_aux_brackets.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 01:52:34 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/24 19:58:57 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_add_while(char *value, int *i, char c, char **new_value)
{
	if (c == 0)
	{
		while (value[*i])
		{
			*new_value = ft_strjoin_char(*new_value, value[*i]);
			if (!(*new_value))
				return (0);
			(*i)++;
		}
		return (1);
	}
	while (value[*i] && (value[*i] != c
			|| (value[*i - 1] && value[*i - 1] == '\\')))
	{
		*new_value = ft_strjoin_char(*new_value, value[*i]);
		if (!(*new_value))
			return (0);
		(*i)++;
	}
	return (1);
}

int	ft_add_bracket_token2(t_token **tmp, t_token *prev, int *i, t_token **token)
{
	char	*str;
	int		start;
	t_token	*new_token;

	str = ft_strdup((*tmp)->str);
	start = 0;
	if (str[*i] == '(' && (!*i || str[*i - 1] != '\\'))
	{
		start = *i;
		new_token = ft_create_token("(", O_BRACKET);
		if (!new_token)
			return (ft_safe_free((void **)&str), 0);
		ft_connect_token(token, new_token, prev);
		ft_safe_free((void **)&((*tmp)->str));
		(*tmp)->str = ft_substr(str, start + 1,
				ft_strlen(str) - (start + 1));
		(*i)--;
		return (ft_safe_free((void **)&str), 2);
	}
	return (ft_safe_free((void **)&str), 1);
}

int	ft_add_bracket_token3(t_token **tmp, int *i, t_token **token)
{
	char	*str;
	t_token	*new_token;

	str = ft_strdup((*tmp)->str);
	if (str[*i] == ')' && (!*i || str[(*i) - 1] != '\\'))
	{
		ft_safe_free((void **)&((*tmp)->str));
		(*tmp)->str = ft_substr(str, 0, *i);
		while (str[*i]
			&& str[*i] == ')' && (!*i || str[(*i) - 1] != '\\'))
		{
			new_token = ft_create_token(")", C_BRACKET);
			if (!new_token)
				return (ft_safe_free((void **)&str), 0);
			ft_connect_token(token, new_token, *tmp);
			(*tmp) = (*tmp)->next;
			(*i)++;
		}
		if (!str[*i])
			return (ft_safe_free((void **)&str), 2);
	}
	return (ft_safe_free((void **)&str), 1);
}

int	ft_add_brackets_loop(t_token *prev, t_token **token, t_token **tmp)
{
	int		i;
	int		res;
	char	*str;

	i = 0;
	str = (*tmp)->str;
	while (str[i])
	{
		res = ft_add_bracket_token2(tmp, prev, &i, token);
		if (!res)
			return (0);
		if (res == 1)
		{
			res = ft_add_bracket_token3(tmp, &i, token);
			if (res == 0)
				return (0);
			if (res == 2)
				break ;
		}
		i++;
		str = (*tmp)->str;
	}
	return (1);
}

int	ft_add_bracket_token(t_token **token)
{
	t_token	*tmp;
	t_token	*prev;

	if (!ft_manage_brackets(*token))
		return (0);
	tmp = *token;
	if (!tmp)
		return (1);
	prev = NULL;
	while (tmp)
	{
		if (!ft_add_brackets_loop(prev, token, &tmp))
			return (0);
		prev = tmp;
		tmp = tmp->next;
	}
	ft_last(token);
	return (1);
}
