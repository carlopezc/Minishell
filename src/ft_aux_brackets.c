/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_aux_brackets.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 01:52:34 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/21 20:36:17 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_add_bracket_token2(t_token **tmp, t_token *prev, int *i, t_token **token)
{
	char	*str;
	int		start;
	t_token	*new_token;

	str = (*tmp)->str;
	start = 0;
	if (str[*i] == '(' && (*i == 0 || str[*i - 1] != '\\'))
	{
		start = *i;
		new_token = ft_create_token("(", O_BRACKET);
		if (!new_token)
			return (0);
		ft_connect_token(token, new_token, prev);
		(*tmp)->str = ft_substr(str, start + 1,
				ft_strlen(str) - (start + 1));
		str = (*tmp)->str;
		(*i)--;
		return (2);
	}
	return (1);
}

int	ft_add_bracket_token3(t_token **tmp, int *i, t_token **token)
{
	char	*str;
	t_token	*new_token;
	char	*cpy;

	str = (*tmp)->str;
	cpy = NULL;
	if (str[*i] == ')' && (*i == 0 || str[(*i) - 1] != '\\'))
	{
		cpy = ft_strdup(str);
		(*tmp)->str = ft_substr(str, 0, *i);
		while (tmp && cpy[*i]
			&& cpy[*i] == ')' && (*i == 0 || cpy[(*i)] != '\\'))
		{
			new_token = ft_create_token(")", C_BRACKET);
			if (!new_token)
				return (0);
			ft_connect_token(token, new_token, *tmp);
			(*tmp) = (*tmp)->next;
			(*i)++;
		}
		if (!cpy[*i])
			return (2);
	}
	return (1);
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
