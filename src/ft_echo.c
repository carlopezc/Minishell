/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:48:46 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/25 13:53:15 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_sort_list(t_env *head)
{
	t_env		*ptr;
	t_env		*lptr;
	int			swapped;

	if (!head)
		return ;
	swapped = 1;
	lptr = NULL;
	while (swapped)
	{
		swapped = 0;
		ptr = head;
		while (ptr->next != lptr)
		{
			if (ft_strncmp(ptr->name, ptr->next->name,
					ft_max_strlen(ptr->name, ptr->next->name)) > 0)
			{
				ft_swap_list(ptr, ptr->next);
				swapped = 1;
			}
			ptr = ptr->next;
		}
		lptr = ptr;
	}
}

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

int	ft_echo_flag(char *str, int *n)
{
	if (!str)
		return (0);
	if (str[*n] && str[*n] == '-' && str[*n + 1] == 'n')
		(*n) += 2;
	while (str[*n] && str[*n] == 'n')
		(*n)++;
	if (!str[*n] || str[*n] == ' ')
	{
		while (str[*n] && str[*n] == ' ')
			(*n)++;
		return (1);
	}
	return (0);
}

void	ft_echo(char *cmd)
{
	char	*temp;
	int		n;

	n = 0;
	temp = ft_strchr(cmd, ' ');
	if (!temp)
		ft_printf("\n");
	if (!ft_strncmp(temp + 1, "-n", 2) && ft_echo_flag(temp + 1, &n))
		ft_echo_print(temp + n + 1);
	else
	{
		ft_echo_print(temp + 1);
		ft_printf("\n");
	}
	return ;
}
