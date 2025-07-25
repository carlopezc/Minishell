/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_brackets.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 05:51:14 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/25 19:38:33 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

static int	ft_check_brackets2(char **value, int *i, int *open)
{
	int	flag;

	flag = 0;
	*i = 0;
	*open = 0;
	while ((*value)[*i] && ((*value)[*i] == '('
			&& (!*i || (*value)[(*i) - 1] != '\\')))
	{
		(*i)++;
		(*open)++;
	}
	while ((*value)[*i] && ((*value)[*i] != '(' && (*value)[*i] != ')'))
	{
		if ((*value)[*i] != ' ')
			flag = 1;
		(*i)++;
	}
	return (flag);
}

int	ft_check_brackets(t_token *token)
{
	char	*value;
	int		i;
	int		open;
	int		close;
	int		flag;

	value = token->str;
	close = 0;
	flag = ft_check_brackets2(&value, &i, &open);
	if (value[i] && ((value[i] == '('
				&& (!i || value[i - 1] != '\\')) || !flag))
		return (ft_printf("syntax error\n"), 0);
	while (value[i] && (value[i] == ')'
			&& (!i || value[i - 1] != '\\')))
	{
		i++;
		close++;
	}
	while (value[i] && value[i] == ' ')
		i++;
	if (close && value[i])
		return (ft_printf("syntax error\n"), 0);
	else if (open && close)
		ft_quit_brackets(token, &open, &close);
	return (1);
}
