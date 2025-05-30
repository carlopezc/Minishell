/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_manage_nodes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 17:02:29 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/30 17:10:21 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

t_env	*ft_create_node(char *name, char *value)
{
	t_env	*node;

	if (!name)
		return (NULL);
	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->name = name;
	if (!value)
		node->value = NULL;
	else
		node->value = value;
	node->next = NULL;
	return (node);
}

void	ft_connect_node(t_env **env, t_env *node)
{
	t_env	*tmp;

	if (!*env)
	{
		*env = node;
		return ;
	}
	tmp = *env;
	while (tmp && tmp->next)
		tmp = tmp->next;
	tmp->next = node;
	return ;
}
