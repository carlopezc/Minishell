/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_aux.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 21:49:31 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/26 16:49:02 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_free_env(t_env **env)
{
	t_env	*tmp;
	t_env	*next;

	if (!env)
		return ;
	tmp = *env;
	while (tmp)
	{
		next = tmp->next;
		ft_safe_free((void **)&tmp->name);
		tmp->name = NULL;
		ft_safe_free((void **)&tmp->value);
		tmp->value = NULL;
		ft_safe_free((void **)&tmp);
		tmp = NULL;
		tmp = next;
	}
	*env = NULL;
	return ;
}

void	ft_free_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		ft_safe_free((void **)&arr[i++]);
	free(arr);
	return ;
}

void	ft_safe_free(void **p)
{
	if (p && *p)
	{
		free(*p);
		*p = NULL;
	}
	return ;
}
