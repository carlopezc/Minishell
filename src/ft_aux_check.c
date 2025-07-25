/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_aux_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 18:31:30 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/25 12:54:04 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_last_check2(int o_brckt, t_token *tmp, t_token *tokens)
{
	int	res;

	res = 0;
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
	return (1);
}

int	ft_last_check(t_token *tokens)
{
	t_token	*tmp;
	int		o_brckt;
	int		i;

	tmp = tokens;
	o_brckt = 0;
	i = 0;
	while (tmp)
	{
		while (tmp->str && (tmp->str)[i])
		{
			if ((tmp->str)[i] && (tmp->str)[i] == '(' && ((!i
				|| (tmp->str)[i - 1] != '\\')))
				o_brckt++;
			i++;
		}
		if (!ft_last_check2(o_brckt, tmp, tokens))
			return (0);
		o_brckt = 0;
		i = 0;
		tmp = tmp->next;
	}
	return (1);
}

int	ft_brackets_loop(char *value, int *i, char c)
{
	int	count;

	count = 0;
	while (value[*i] && (value[*i] == c
			&& (*i == 0 || value[*i - 1] != '\\')))
	{
		(*i)++;
		count++;
	}
	return (count);
}

int	ft_is_builtin2(char *input, int flag, int i)
{
	if (!ft_strncmp(input + flag + i, "unset", 6)
		|| !ft_cmp_except(input + flag + i, "unset", 6))
		return (1);
	else if (!ft_strncmp(input + flag + i, "env", 4)
		|| !ft_cmp_except(input + flag + i, "env", 4))
		return (1);
	else if (!ft_strncmp(input + flag + i, "exit", 5)
		|| !ft_cmp_except(input + flag + i, "exit", 5))
		return (1);
	return (0);
}

int	ft_is_builtin(char *input)
{
	int	flag;
	int	i;

	flag = 0;
	i = 0;
	if (*input == '(')
		flag = 1;
	ft_skip_spaces(input + flag, &i);
	if (!ft_strncmp(input + flag + i, "echo", 5)
		|| !ft_cmp_except(input + flag + i, "echo", 5))
		return (1);
	else if (!ft_strncmp(input + flag + i, "cd", 3)
		|| !ft_cmp_except(input + flag + i, "cd", 3))
		return (1);
	else if (!ft_strncmp(input + flag + i, "pwd", 4)
		|| !ft_cmp_except(input + flag + i, "pwd", 4))
		return (1);
	else if (!ft_strncmp(input + flag + i, "export", 7)
		|| !ft_cmp_except(input + flag + i, "export", 7))
		return (1);
	if (ft_is_builtin2(input, flag, i))
		return (1);
	return (0);
}
