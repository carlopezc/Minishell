/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor16.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 20:03:24 by lbellmas          #+#    #+#             */
/*   Updated: 2025/07/26 05:00:02 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_finish_build(int num, t_minishell *shell)
{
	int	pid;

	pid = fork();
	if (pid == 0)
	{
		ft_free_minishell(&shell);
		exit(num);
	}
	waitpid(pid, 0, 0);
	shell->status = num;
}

int	ft_check_pipes(t_token *save, t_minishell *shell)
{
	t_token	*prev;

	prev = shell->tokens;
	while (prev != save && prev->next != save)
		prev = prev->next;
	if (prev->type == PIPE)
		return (1);
	while (save)
	{
		if (save->type == PIPE)
			return (1);
		else if (save->type == AND || save->type == OR)
			return (0);
		save = save->next;
	}
	return (0);
}

void	ft_exit(t_pipex *pipex, t_minishell *shell, t_token *save)
{
	unsigned int	temp;

	temp = 0;
	if (pipex->brackets_count || ft_check_pipes(save, shell)
		|| pipex->pipe[0][0])
		return (ft_finish_build(0, shell));
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
