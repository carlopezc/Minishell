/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:37:21 by lbellmas          #+#    #+#             */
/*   Updated: 2025/07/23 20:24:37 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../get_next_line/get_next_line_bonus.h"
#include "../header/ft_minishell.h"
#include <fcntl.h>
#include <sys/wait.h>

t_token	*ft_and(t_pipex *pipex, t_minishell *shell, t_token *save)
{
	int	brackets;

	brackets = pipex->brackets_count;
	while (pipex->childs > 0)
	{
		wait(&shell->status);
		if (WIFEXITED(shell->status))
			shell->status = WEXITSTATUS(shell->status);
		pipex->childs--;
	}
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
	while (pipex->childs > 0)
	{
		wait(&shell->status);
		if (WIFEXITED(shell->status))
			shell->status = WEXITSTATUS(shell->status);
		pipex->childs--;
	}
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

static t_token	*ft_executor2(t_pipex *pipex, t_token *save, t_minishell *shell,
	t_token *tmp)
{
	if (ft_strncmp("exit", save->str, 4) == 0)
	{
		ft_free_pipex(&pipex);
		ft_free_minishell(&shell);
		exit(0);
	}
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
