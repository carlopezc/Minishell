/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor6.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 18:13:20 by lbellmas          #+#    #+#             */
/*   Updated: 2025/05/31 19:21:39 by lbellmas         ###   ########.fr       */
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
	t_env	*ptr;
	t_env	*lptr;
	int		swapped;

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

int	ft_check_name(char *var)
{
	int	i;

	i = 0;
	if (!ft_isalpha(var[i]) && (var[i] != '_'))
		return (ft_printf("Non valid name\n"), 0);
	while (var[++i] && var[i] != '=')
	{
		if (!ft_isalnum(var[i]) && (var[i] != '_'))
			return (ft_printf("Non valid name\n"), 0);
	}
	return (1);
}

static int	ft_export2(char **var, t_env *node, t_minishell *shell, int i)
{
	int	flag;

	flag = 0;
	if (!ft_check_name(var[i]))
		return (-1);
	if (ft_strchr(var[i], '='))
	{
		if (!ft_check_duplicated(var[i], &shell->env, &shell->undefined_var))
		{
			node = ft_create_node(ft_get_name(var[i]), ft_get_value(var[i]));
			ft_connect_node(&shell->env, node);
			flag = 1;
		}
	}
	if (!flag)
	{
		if (!ft_check_duplicated(var[i], &shell->env, &shell->undefined_var))
		{
			node = ft_create_node(ft_get_name(var[i]), NULL);
			ft_connect_node(&shell->undefined_var, node);
		}
	}
	return (i + 1);
}

void	ft_export(t_minishell *shell, char *cmd)
{
	int		i;
	char	**var;
	t_env	*node;

	node = NULL;
	var = ft_split(cmd, ' ');
	if (!var)
		return ;
	i = 0;
	if (!ft_strncmp(var[i], "export",
			ft_max_strlen(var[i], "export")) && !var[++i])
		return (ft_sort_list(shell->export), ft_print_export(shell->export));
	while (i != -1 && var[i])
		i = ft_export2(var, node, shell, i);
	ft_merge_lists(&shell, shell->env, shell->undefined_var);
	ft_sort_list(shell->export);
	ft_print_export(shell->export);
	return ;
}
