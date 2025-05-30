/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup_variants.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:09:57 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/29 18:48:08 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

t_env	*ft_strdup_env(t_env *env)
{
	t_env	*cpy;
	t_env	*node;
	t_env	*tmp;

	if (!env)
		return (NULL);
	cpy = NULL;
	tmp = env;
	while (tmp)
	{
		node = ft_create_node(ft_strdup(tmp->name), ft_strdup(tmp->value));
		if (!node)
			return (ft_free_env(&cpy), NULL);
		ft_connect_node(&cpy, node);
		tmp = tmp->next;
	}
	return (cpy);
}
