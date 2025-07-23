/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:13:37 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/23 12:49:46 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_change_value(char *str, t_env **node)
{
	char	*equal;
	int	value;
	char	*str_value;
	char	*name;

	equal = ft_strchr(str, '=');
	value = 0;
	name = ft_get_name(str);
	str_value = ft_get_value(str);
	if (*(equal - 1) == '+')
		(*node)->value = ft_strjoin((*node)->value, str_value);
	else
	{
		if (!ft_strncmp(name, "SHLVL", 6))
		{
			value = ft_atoi(str_value);
			if (value > 999)
				value = 1;
			(*node)->value = ft_itoa(value);
		}
		else
		{	
			ft_safe_free((void **)&((*node)->value));
			(*node)->value = str_value;
		}
	}
	ft_safe_free((void **)&str_value);
	ft_safe_free((void **)&name);
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
	char	*name;
	char	*value;

	name = NULL;
	value = NULL;
	if (ft_strchr(split[i], '='))
	{
		if (!ft_check_duplicated(split[i], &shell->env, &shell->undefined_var))
		{
			name = ft_get_name(split[i]);
			value = ft_get_value(split[i]);
			*node = ft_create_node(name, value);
			ft_connect_node(&shell->env, *node);
			//ft_safe_free((void **)&name);
			//ft_safe_free((void **)&value);
		}
		return (1);
	}
	return (0);
}

static int	ft_export_loop(char **split, t_minishell *shell)
{
	int		i;
	t_env	*node;
	char	*name;

	i = 1;
	name = NULL;
	while (split[i])
	{
		if (!ft_check_name(split[i]))
			return (0);
		if (!ft_export_aux(split, i, shell, &node))
		{
			if (!ft_check_duplicated(split[i],
					&shell->env, &shell->undefined_var))
			{
				name = ft_get_name(split[i]);
				node = ft_create_node(name, NULL);
				ft_connect_node(&shell->undefined_var, node);
				ft_safe_free((void **)name);
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
		return (ft_free_array(split));
	if (!ft_strncmp(split[i], "export",
			ft_max_strlen(split[i], "export")) && !split[i + 1])
		return (ft_free_array(split), ft_print_export(shell));
	if (!ft_export_loop(split, shell))
		return (ft_free_array(split));
	ft_merge_lists(&shell, shell->env, shell->undefined_var);
	ft_sort_list(shell->export);
	return (ft_free_array(split));
}
