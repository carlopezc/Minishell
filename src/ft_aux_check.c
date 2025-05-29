/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_aux_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 18:31:30 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/29 18:40:54 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_check_next(t_token *tokens, int o_brckt)
{
	t_token	*tmp;
	int		c_brckt;
	int		i;

	tmp = tokens;
	c_brckt = 0;
	i = 0;
	while (tmp)
	{
		while ((tmp->str)[i])
		{
			if ((tmp->str)[i] && (tmp->str)[i] == ')' && ((!(tmp->str)[i - 1]
								|| (tmp->str)[i - 1] != '\\')))
				c_brckt++;
			i++;
		}
		if (c_brckt == o_brckt)
		{
			if (!ft_quit_brckt_dup(tmp, ')'))
				return (0);
			return (2);
		}
		c_brckt = 0;
		i = 0;
		tmp = tmp->next;
	}
	return (1);
}

int	ft_last_check(t_token *tokens)
{
	t_token	*tmp;
	int		o_brckt;
	int		i;
	int		res;

	tmp = tokens;
	o_brckt = 0;
	i = 0;
	res = 0;
	while (tmp)
	{
		while ((tmp->str)[i])
		{
			if ((tmp->str)[i] && (tmp->str)[i] == '(' && ((!(tmp->str)[i - 1]
									|| (tmp->str)[i - 1] != '\\')))
				o_brckt++;
			i++;
		}
		if (o_brckt > 1)
		{
			res = ft_check_next(tokens, o_brckt);
			if (res == 0)
				return (0);
			else if (res == 2)
			{
				if (!ft_quit_brckt_dup(tmp, '('))
					return (0);
			}
		}
		o_brckt = 0;
		i = 0;
		tmp = tmp->next;
	}
	return (1);
}

int	ft_check_brackets(t_token *token)
{
	char	*value;
	int		i;
	int		open;
	int		close;

	value = token->str;
	i = 0;
	open = 0;
	close = 0;
	while (value[i] && (value[i] == '('
			&& (!value[i - 1] || value[i - 1] != '\\')))
	{
		i++;
		open++;
	}
	while (value[i] && (value[i] != '(' && value[i] != ')'))
		i++;
	while (value[i] && (value[i] == ')'
			&& (!value[i - 1] || value[i - 1] != '\\')))
	{
		i++;
		close++;
	}
	while (value[i] && value[i] == ' ')
		i++;
	if (close && value[i])
		return (ft_printf("parse error near blabla \n"), 0);
	else if (open && close)
		ft_quit_brackets(token, &open, &close);
	return (1);
}
