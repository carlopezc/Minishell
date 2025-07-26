/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor6.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 18:13:20 by lbellmas          #+#    #+#             */
/*   Updated: 2025/07/26 03:24:21 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_advance_var(char *cmd, int *i)
{
	t_quote	quote;

	ft_init_quote(&quote);
	if (!cmd)
		return ;
	while (cmd[*i] && cmd[*i] == ' ')
		(*i)++;
	while (cmd[*i] && cmd[*i] != ' ')
	{
		if (cmd[*i] == '=')
			break ;
		(*i)++;
	}
	if (cmd[*i] != '=')
		return ;
	while (cmd[*i])
	{
		ft_check_quote(&quote, cmd[*i]);
		if (cmd[*i] == ' ' && quote.flag == 0)
			return ;
		(*i)++;
	}
	return ;
}

int	ft_check_name(char *var)
{
	int	i;

	i = 0;
	if (!ft_isalpha(var[i]) && (var[i] != '_')
		&& (var[i] != '\'') && (var[i] != '\"'))
	{
		ft_printf("Not a valid identifier\n");
		return (0);
	}
	while (var[++i] && (var[i] != '=' && var[i] != ' '))
	{
		if (var[i] == '+' && var[i + 1] == '=')
			return (1);
		if (!ft_isalnum(var[i]) && (var[i] != '_')
			&& (var[i] != '\'') && (var[i] != '\"'))
		{
			ft_printf("Not a valid identifier\n");
			return (0);
		}
	}
	return (1);
}
