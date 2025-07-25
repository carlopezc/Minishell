/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:15:04 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/25 22:20:32 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_init_quote(t_quote *quote)
{
	if (quote)
	{
		quote->type = 0;
		quote->flag = 0;
	}
	return ;
}

t_pipex	*ft_init_pipex(void)
{
	t_pipex	*pipex;

	pipex = (t_pipex *)malloc(sizeof(t_pipex));
	if (!pipex)
		return (NULL);
	pipex->childs = 0;
	pipex->docs_in = NULL;
	pipex->docs_out = NULL;
	pipex->command = NULL;
	pipex->path = NULL;
	pipex->pid = 0;
	pipex->brackets_count = 0;
	pipex->heredoc = 0;
	pipex->pipe[0][0] = 0;
	pipex->pipe[0][1] = 0;
	pipex->pipe[1][0] = 0;
	pipex->pipe[1][1] = 0;
	return (pipex);
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
	(*minishell)->status = 0;
	if (env && *env)
	{
		(*minishell)->env = ft_create_env(env);
		if (!(*minishell)->env)
			return (ft_free_minishell(minishell),
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
