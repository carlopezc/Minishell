/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:15:04 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/29 18:01:47 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_init_quote(t_quote *quote)
{
	if (*quote)
	{
		quote->type = 0;
		quote->flag = 0;
	}
	return ;
}

int	ft_init_minishell(t_minishell **minishell, char **env)
{
	*minishell = (t_minishell *)malloc(sizeof(t_minishell));
	if (!(*minishell))
		return (0);
	(*minishell)->tokens = NULL;
	(*minishell)->env_tmp = NULL;
	(*minishell)->undefined_var = NULL;
	(*minishell)->export = NULL;
	if (env && *env)
	{
		(*minishell)->env = ft_create_env(env);
		if (!(*minishell)->env)
			return (free(*minishell),
				ft_printf("Error creating environment \n"), 0);
		(*minishell)->export = ft_create_env(env);
	}
	else
	{
		(*minishell)->env = ft_empty_env();
		ft_empty_export(minishell);
	}
	if (!(*minishell)->export)
		return (ft_free_minishell(minishell),
			ft_printf("Error creating export \n"), 0);
	return (1);
}
