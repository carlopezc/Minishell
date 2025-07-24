/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor16.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 20:03:24 by lbellmas          #+#    #+#             */
/*   Updated: 2025/07/24 20:04:15 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"
#include <fcntl.h>
#include <sys/wait.h>

void	ft_exit(t_pipex *pipex, t_minishell *shell, t_token *save)
{
	unsigned int	temp;

	temp = 0;
	if (ft_strlen(save->str) > 5)
		temp = ft_atoi(save->str + 4);
	ft_free_pipex(&pipex);
	ft_free_minishell(&shell);
	exit(temp);
}

void	ft_check_exit(t_pipex *pipex, t_minishell *shell)
{
	int	temp;
	int	sig;

	while (pipex->childs > 0)
	{
		wait(&temp);
		if (WIFEXITED(temp))
			shell->status = WEXITSTATUS(temp);
		pipex->childs--;
	}
	if (WIFSIGNALED(temp))
	{
		sig = WTERMSIG(temp);
		if (sig == SIGINT)
			write(1, "\n", 1);
		else if (sig == SIGQUIT)
			write(1, "Quit (core dumped)\n", 19);
	}
}
