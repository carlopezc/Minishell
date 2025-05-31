/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:13:37 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/30 19:48:44 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

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
