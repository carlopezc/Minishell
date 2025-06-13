/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor6.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 18:13:20 by lbellmas          #+#    #+#             */
/*   Updated: 2025/06/13 11:27:22 by carlotalcd       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"
#include <fcntl.h>
#include <sys/wait.h>

size_t	ft_max_strlen(char *s1, char *s2)
{
	size_t	len1;
	size_t	len2;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	if (len1 > len2)
		return (len1);
	else
		return (len2);
}

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
	if (!ft_isalpha(var[i]) && (var[i] != '_'))
	{
		ft_printf("Non valid name\n");
		return (0);
	}
	while (var[++i] && (var[i] != '=' && var[i] != ' '))
	{
		if (var[i] == '+' && var[i + 1] == '=')
			return (1);
		if (!ft_isalnum(var[i]) && (var[i] != '_'))
		{
			ft_printf("Non valid name\n");
			return (0);
		}
	}
	return (1);
}
