/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_aux_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 18:31:30 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/21 20:35:32 by carlopez         ###   ########.fr       */
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
		while ((tmp->str)[i])
		{
			if ((tmp->str)[i] && (tmp->str)[i] == '(' && ((i == 0 
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

int	ft_check_brackets(t_token *token)
{
	char	*value;
	int		i;
	int		open;
	int		close;
	int		flag;

	value = token->str;
	i = 0;
	open = ft_brackets_loop(value, &i, '(');
	close = 0;
	flag = 0;
	while (value[i] && (value[i] != '(' && value[i] != ')'))
	{
		flag = 1;
		i++;
	}
	if (value[i] && value[i] == '(' && (i == 0 || value[i - 1] != '\\'))
		return (ft_printf("syntax error near unsexpected token ')'\n"), 0);
	close = ft_brackets_loop(value, &i, ')');
	if (!flag && open == close)
		return (ft_printf("syntax error near unexpected token ')'\n"), 0);
	while (value[i] && value[i] == ' ')
		i++;
	if (close && value[i])
		return (ft_printf("syntax error near unsexpected token blabla\n"), 0);
	else if (open && close)
		ft_quit_brackets(token, &open, &close);
	return (1);
}

int	ft_is_builtin(char *input)
{
	if (*input == '(')
		input = input + 1;
	if (!ft_strncmp(input, "echo", 5) || !ft_strncmp(input, "echo ", 5))
		return (1);
	else if (!ft_strncmp(input, "cd", 3) || !ft_strncmp(input, "cd ", 3))
		return (1);
	else if (!ft_strncmp(input, "pwd", 4) || !ft_strncmp(input, "pwd ", 4))
		return (1);
	else if (!ft_strncmp(input, "export", 7) || !ft_strncmp(input, "export ", 7))
		return (1);
	else if (!ft_strncmp(input, "unset", 6) || !ft_strncmp(input, "unset ", 6))
		return (1);
	else if (!ft_strncmp(input, "env", 4) || !ft_strncmp(input, "env ", 4))
		return (1);
	else if (!ft_strncmp(input, "exit", 5) || !ft_strncmp(input, "exit ", 5))
		return (1);
	else
		return (0);
}
