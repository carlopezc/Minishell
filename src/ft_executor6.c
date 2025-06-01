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

void	ft_advance_var(char *cmd, int *i)
{
	t_quote quote;

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
		return (ft_printf("Non valid name\n"), 0);
	while (var[++i] && (var[i] != '=' && var[i] != ' '))
	{
		if (var[i] == '+' && var[i + 1] == '=')
			return (1);
		if (!ft_isalnum(var[i]) && (var[i] != '_'))
			return (ft_printf("Non valid name\n"), 0);
	}
	return (1);
}

void	ft_export(t_minishell *shell, char *cmd)
{
	int	i;
	int flag;
	t_env *node;
	char	**split;

	i = 0;
	split = ft_split_cmd(cmd, ' ');
	if (!split || !*split)
		return (ft_free_todo(i, split));
	if (!ft_strncmp(split[i], "export", ft_max_strlen(split[i], "export")) && !split[i + 1])
		return (ft_sort_list(shell->export), ft_print_export(shell->export));
	while (split[i])
	{
		if (!ft_check_name(split[i]))
			return ;
		if (ft_strchr(split[i], '='))
		{
			if (!ft_check_duplicated(split[i], &shell->env, &shell->undefined_var))
			{
				node = ft_create_node(ft_get_name(split[i]), ft_get_value(split[i]));
				ft_connect_node(&shell->env, node);
			}
			flag = 1;
		}
		else if (!flag)
		{
			if (!ft_check_duplicated(split[i], &shell->env, &shell->undefined_var))
			{
				node = ft_create_node(ft_get_name(split[i]), NULL);
				ft_connect_node(&shell->undefined_var, node);
			}
		}
		flag = 0;
		i++;
	}
	ft_merge_lists(&shell, shell->env, shell->undefined_var);
	ft_sort_list(shell->export);
	//ft_print_export(shell->export);
	return ;
}

/*
static int	ft_export2(char **split, t_env *node, t_minishell *shell, int i)
{
	int	flag;

	flag = 0;
	if (!ft_check_name(split[i]))
		return (-1);
	if (ft_strchr(split[i], '='))
	{
		if (!ft_check_duplicated(split[i], &shell->env, &shell->undefined_var))
		{
			node = ft_create_node(ft_get_name(split[i]), ft_get_value(split[i]));
			ft_connect_node(&shell->env, node);
		}
		flag = 1;
	}
	if (!flag)
	{
		if (!ft_check_duplicated(split[i], &shell->env, &shell->undefined_var))
		{
			node = ft_create_node(ft_get_name(split[i]), NULL);
			ft_connect_node(&shell->undefined_var, node);
		}
	}
	return (i + 1);
}

void	ft_export(t_minishell *shell, char *cmd)
{
	int		i;
	char	**split;
	t_env	*node;

	node = NULL;
	split = ft_split_cmd(cmd, ' ');
	if (!split)
		return ;
	i = 0;
	if (!ft_strncmp(split[i], "export",
			ft_max_strlen(split[i], "export")) && !split[++i])
		return (ft_sort_list(shell->export), ft_print_export(shell->export));
	while (i != -1 && split[i])
		i = ft_export2(split, node, shell, i);
	ft_merge_lists(&shell, shell->env, shell->undefined_var);
	ft_sort_list(shell->export);
	return ;
}*/
