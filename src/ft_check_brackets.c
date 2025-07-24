/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_brackets.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 05:51:14 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/24 06:04:25 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_check_mid_brackets(char *value, int *i, int *flag)
{
	while (value[*i] && value[*i] != '(' && value[*i] != ')')
	{
		*flag = 1;
		(*i)++;
	}
	if (value[*i] == '(' && (*i == 0 || value[*i - 1] != '\\'))
	{
		ft_printf("syntax error near unexpected token ')'\n");
		return (0);
	}
	return (1);
}

int	ft_check_end_brackets(char *value, int i, int *data)
{
	while (value[i] && value[i] == ' ')
		i++;
	if (data[1] && value[i])
	{
		ft_printf("syntax error near unexpected token after brackets\n");
		return (0);
	}
	if (!data[2] && data[0] == data[1])
	{
		ft_printf("syntax error near unexpected token ')'\n");
		return (0);
	}
	return (1);
}

int	ft_check_brackets(t_token *token)
{
	char	*value;
	int		i;
	int		data[3];

	value = token->str;
	i = 0;
	data[0] = ft_brackets_loop(value, &i, '(');
	data[2] = 0;
	if (!ft_check_mid_brackets(value, &i, &data[2]))
		return (0);
	data[1] = ft_brackets_loop(value, &i, ')');
	if (!ft_check_end_brackets(value, i, data))
		return (0);
	if (data[0] && data[1])
		ft_quit_brackets(token, &data[0], &data[1]);
	return (1);
}
