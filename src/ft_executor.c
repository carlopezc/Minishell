/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:37:21 by lbellmas          #+#    #+#             */
/*   Updated: 2025/03/25 11:13:12 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_executor(t_minishell *shell)
{
	int	p;
	t_pipex	*pipex;

	p = 0;
	while (shell[p])
	{
		while (shell->tokens[p++]->type != pipe)
		{
			if (shell->tokens[p]->type == command)
			{
				ft_path(shell->env, pipex, shell->tokens[p]->str);
				pipex->pid = fork();
				p++;
			}
			if (pipex->pid == 0 && shell->tokens[p++]->type == redirect_infile)
				pipex->docs[0] = open(shell->tokens[p++]->type, O_RDONLY);
			if (pipex->pid == 0 && shell->tokens[p++]->type == redirect_outfile)
				pipex->docs[1] = open(shell->tokens[p++]->type, O_WRONLY);
			else if (shell->tokens[p] && pipe(pipex->pipe[1]))
				return (-1);
			if (pipex->pid == 0)
				ft_exec();
			else
			{
				if (shell->tokens[p]->type == redirect_infile)
					p++;
				if (shell->tokens[p]->type == redirect_outfile)
					p++;
			}
		}
		p++;
		ft_arrange_fd();
	}
}
