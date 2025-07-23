/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor15.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 23:59:05 by lbellmas          #+#    #+#             */
/*   Updated: 2025/07/24 00:01:17 by lbellmas         ###   ########.fr       */
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
		else if ((cmd[i] == '\"' || cmd[i] == '\'')
			&& (!i || cmd[i - 1] != '\\'))
		{
			if (q.flag && q.type != cmd[i])
				ft_printf("%c", cmd[i]);
			ft_check_quote(&q, cmd[i]);
			i++;
		}
		else
		{
			if (cmd[i] == '\\')
				check = 1;
			ft_printf("%c", cmd[i++]);
		}
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
