/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:37:21 by lbellmas          #+#    #+#             */
/*   Updated: 2025/07/24 20:04:24 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"
#include <fcntl.h>
#include <sys/wait.h>
/*
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
}*/

t_token	*ft_and(t_pipex *pipex, t_minishell *shell, t_token *save)
{
	int	brackets;

	brackets = pipex->brackets_count;
	ft_check_exit(pipex, shell);
	if (shell->status != 0)
	{
		while (save && (save->type != AND || pipex->brackets_count <= brackets)
			&& (save->type != OR || brackets != pipex->brackets_count))
		{
			if (save->type == O_BRACKET)
				pipex->brackets_count += 1;
			if (save->type == C_BRACKET)
				pipex->brackets_count -= 1;
			save = save->next;
		}
	}
	return (save);
}

t_token	*ft_or(t_pipex *pipex, t_minishell *shell, t_token *save)
{
	int	brackets;

	brackets = pipex->brackets_count;
	ft_check_exit(pipex, shell);
	if (shell->status == 0)
	{
		while (save && (save->type != AND
				|| !(pipex->brackets_count <= brackets))
			&& (save->type != OR || brackets != pipex->brackets_count))
		{
			if (save->type == O_BRACKET)
				pipex->brackets_count += 1;
			if (save->type == C_BRACKET)
				pipex->brackets_count -= 1;
			save = save->next;
		}
	}
	return (save);
}
/*
void	ft_exit(t_pipex *pipex, t_minishell *shell, t_token *save)
{
	unsigned int	temp;

	temp = 0;
	if (ft_strlen(save->str) > 5)
		temp = ft_atoi(save->str + 4);
	ft_free_pipex(&pipex);
	ft_free_minishell(&shell);
	exit(temp);
}*/

static t_token	*ft_executor2(t_pipex *pipex, t_token *save, t_minishell *shell,
	t_token *tmp)
{
	if (!ft_strncmp("exit", save->str, 5) || !ft_strncmp("exit ", save->str, 5))
		ft_exit(pipex, shell, save);
	if (save->type == COMMAND || save->type == BUILTIN
		|| save->type == EXEC || save->type == HEREDOC)
		tmp = ft_analisis_comands(pipex, shell, &save);
	while (save && (save->type == REDIR_IN || save->type
			== REDIR_OUT || save->type == HEREDOC || save->type == APPEND))
		save = ft_analisis_redir(save, pipex, tmp);
	save = ft_exectime(pipex, shell, tmp, save);
	if (save && save->type == C_BRACKET)
	{
		pipex->brackets_count -= 1;
		save = save->next;
	}
	return (save);
}

t_token	*ft_executor3(t_token *save, t_pipex *pipex, t_minishell *shell)
{
	t_token	*tmp;

	tmp = NULL;
	while (save && save->type == O_BRACKET)
	{
		pipex->brackets_count += 1;
		save = save->next;
	}
	while (save && save->type != PIPE && save->type != AND && save->type != OR)
		save = ft_executor2(pipex, save, shell, tmp);
	if (save && save->type == PIPE)
		save = save->next;
	ft_arrange_fd(pipex);
	return (save);
}

int	ft_executor(t_minishell *shell)
{
	t_token	*save;
	t_pipex	*pipex;

	pipex = ft_init_pipex();
	save = shell->tokens;
	while (save)
	{
		while (save && save->type != AND && save->type != OR)
			save = ft_executor3(save, pipex, shell);
		save = ft_killchilds(pipex, shell, save);
	}
	return (ft_free_pipex(&pipex), 0);
}
