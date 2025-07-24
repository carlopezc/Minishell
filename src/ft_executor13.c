/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor13.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 18:21:00 by lbellmas          #+#    #+#             */
/*   Updated: 2025/07/24 15:42:46 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"
#include <fcntl.h>
#include <sys/wait.h>

t_token	*ft_killchilds(t_pipex *pipex, t_minishell *shell, t_token *save)
{
	if (save && save->type == AND)
		save = ft_and(pipex, shell, save->next);
	else if (save && save->type == OR)
		save = ft_or(pipex, shell, save->next);
	else
		ft_check_exit(pipex, shell);
	if (save)
		return (save);
	return (ft_terminator(pipex), NULL);
}

void	ft_terminator(t_pipex *pipex)
{
	while (pipex->childs > 0)
	{
		wait(NULL);
		pipex->childs--;
	}
}

t_token	*ft_exectime(t_pipex *pipex, t_minishell *shell,
	t_token *tmp, t_token *save)
{
	if (pipex->pid == 0 && tmp)
	{
		ft_exec(shell, pipex, tmp);
		if (save)
			save = save->next;
	}
	else
	{
		while (save && save->type != PIPE && save->type
			!= AND && save->type != OR)
		{
			if (save->type == C_BRACKET)
				pipex->brackets_count--;
			save = save->next;
		}
	}
	return (save);
}
