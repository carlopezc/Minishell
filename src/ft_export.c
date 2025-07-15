/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:13:37 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/15 11:58:20 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_change_value(char *str, t_env **node)
{
	char	*equal;
	int	value;

	equal = ft_strchr(str, '=');
	value = 0;
	if (*(equal - 1) == '+')
		(*node)->value = ft_strjoin((*node)->value, ft_get_value(str));
	else
	{
		if (!ft_strncmp(ft_get_name(str), "SHLVL", 6))
		{
			value = ft_atoi(ft_get_value(str));
			if (value > 999)
				value = 1;
			(*node)->value = ft_itoa(value);
		}
		else
		{	
			ft_safe_free((void **)&((*node)->value));
			(*node)->value = ft_get_value(str);
		}
	}
	return ;
}

void	ft_empty_export(t_minishell **shell)
{
	t_env	*node;

	node = ft_create_node(ft_strdup("OLDPWD"), NULL);
	if (!node)
		return ;
	ft_connect_node(&(*shell)->undefined_var, node);
	ft_merge_lists(shell, (*shell)->env, (*shell)->undefined_var);
	return ;
}

int	ft_export_aux(char **split, int i, t_minishell *shell, t_env **node)
{
	if (ft_strchr(split[i], '='))
	{
		if (!ft_check_duplicated(split[i], &shell->env, &shell->undefined_var))
		{
			*node = ft_create_node(ft_get_name(split[i]),
					ft_get_value(split[i]));
			ft_connect_node(&shell->env, *node);
			ft_printf("Nodo anadido es %s\n", (*node)->name);
			ft_printf("Nodo anadido es %s\n", (*node)->value);
		}
		return (1);
	}
	return (0);
}

static int	ft_export_loop(char **split, t_minishell *shell)
{
	int		i;
	t_env	*node;

	i = 1;
	while (split[i])
	{
		if (!ft_check_name(split[i]))
			return (ft_free_todo(i, split), 0);
		if (!ft_export_aux(split, i, shell, &node))
		{
			if (!ft_check_duplicated(split[i],
					&shell->env, &shell->undefined_var))
			{
				node = ft_create_node(ft_get_name(split[i]), NULL);
				ft_connect_node(&shell->undefined_var, node);
			}
		}
		i++;
	}
	return (1);
}

void	ft_export(t_minishell *shell, char *cmd)
{
	int				i;
	char			**split;

	i = 0;
	split = ft_split_cmd(cmd, ' ');
	if (!split || !*split)
		return (ft_free_todo(i, split));
	if (!ft_strncmp(split[i], "export",
			ft_max_strlen(split[i], "export")) && !split[i + 1])
		return (ft_free_todo(i, split), ft_print_export(shell));
	if (!ft_export_loop(split, shell))
		return ;
	ft_merge_lists(&shell, shell->env, shell->undefined_var);
	ft_sort_list(shell->export);
	return (ft_free_todo(i, split));
}
