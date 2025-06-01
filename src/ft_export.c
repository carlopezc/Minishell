/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:13:37 by carlopez          #+#    #+#             */
/*   Updated: 2025/06/01 04:38:56 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_print_value(char *value)
{
	int	i;

	i = 0;
	ft_printf("\"");
	while (value[i])
	{
		if (value[i] == '\"' || value[i] == '\'')
			write(1, "\\", 1);
		if (value[i] == '$')
			write(1, "\\", 1);
		if (value[i] == '\\')
			write(1, "\\", 1);
		write(1, &value[i], 1);
		i++;
	}
	ft_printf("\"\n");
	return ;
}

void	ft_print_export(t_env *export)
{
	t_env	*tmp;

	tmp = export;
	while (tmp)
	{
		if (!tmp->value)
			ft_printf("declare -x %s\n", tmp->name);
		else if (tmp->value[0] == '\0')
			ft_printf("declare -x %s=\"\"\n", tmp->name);
		else
		{
			if (!(tmp->name[0] == '_' && tmp->name[1] == '\0'))
			{
				ft_printf("declare -x %s=", tmp->name, tmp->value);
				ft_print_value(tmp->value);
			}
		}
		tmp = tmp->next;
	}
	return ;
}

void	ft_change_value(char *str, t_env **node)
{
	char	*equal;

	equal = ft_strchr(str, '=');
	if (*(equal - 1) == '+')
		(*node)->value = ft_strjoin((*node)->value, ft_get_value(str));
	else
	{
		ft_safe_free((void **)&((*node)->value));
		(*node)->value = ft_get_value(str);
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
