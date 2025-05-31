/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_aux.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 21:49:31 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/30 21:50:01 by carlopez         ###   ########.fr       */
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
		ft_safe_free((void **)&tmp->value);
		ft_safe_free((void **)&tmp);
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
