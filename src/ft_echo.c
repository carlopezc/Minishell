/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:48:46 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/26 21:23:59 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_aux_echo_print(char *cmd, int *i, t_quote *q, int *check)
{
	if ((cmd[*i] == '\"' || cmd[*i] == '\'')
		&& (!(*i) || cmd[(*i) - 1] != '\\'))
	{
		if (q->flag && q->type != cmd[*i])
			ft_printf("%c", cmd[*i]);
		ft_check_quote(q, cmd[*i]);
		(*i)++;
	}
	else
	{
		if (cmd[*i] == '\\')
			*check = 1;
		ft_printf("%c", cmd[(*i)++]);
	}
}

void	ft_echo_print(char *cmd)
{
	int		i;
	t_quote	q;
	int		check;

	i = 0;
	check = 0;
	ft_init_quote(&q);
	if (!cmd)
		return ;
	while (cmd[i])
	{
		if (cmd[i] == '\\'
			&& ((!i || cmd[i - 1] != '\\' || check) && q.type != '\''))
		{
			i++;
			check = 0;
		}
		else
			ft_aux_echo_print(cmd, &i, &q, &check);
	}
}

int	ft_aux_echo_flag(char *str, int *n, int *i)
{
	if (str[*i] && str[*i] != '-')
	{
		*n = *i;
		return (1);
	}
	else
		(*i)++;
	if (!str[(*i)] || str[(*i)] != 'n')
	{
		(*i)--;
		*n = *i;
		return (1);
	}
	while (str[*i] && str[*i] == 'n')
		(*i)++;
	return (0);
}

int	ft_echo_flag(char *str, int *n)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (ft_aux_echo_flag(str, n, &i))
			return (1);
		if (!str[i])
		{
			*n = i;
			return (1);
		}
		if (!str[i] || str[i] != ' ')
			return (1);
		while (str[i] && str[i] == ' ')
			i++;
		*n = i;
	}
	return (1);
}

void	ft_echo(char *cmd)
{
	char	*temp;
	int		n;

	n = 0;
	temp = ft_strchr(cmd, ' ') + 1;
	if (!temp)
		ft_printf("\n");
	if (!ft_strncmp(temp, "-n", 2) && ft_echo_flag(temp, &n))
		ft_echo_print(temp + n);
	else
	{
		ft_echo_print(temp);
		ft_printf("\n");
	}
	return ;
}
