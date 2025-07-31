/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_analisis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 22:16:33 by lbellmas          #+#    #+#             */
/*   Updated: 2025/07/31 18:41:10 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

static int	ft_mega_if(t_token **save)
{
	if ((*save)->type == COMMAND || (*save)->type == EXEC
		|| (*save)->type == HEREDOC
		|| !ft_strncmp("pwd", (*save)->str, 3)
		|| !ft_strncmp("echo", (*save)->str, 4)
		|| ((!ft_strncmp("env", (*save)->str, 3)
				&& ft_strncmp("env ", (*save)->str, 4))
			|| (!ft_strncmp("export", (*save)->str, 6)
				&& ft_strncmp("export ", (*save)->str, 7))))
		return (1);
	return (0);
}

void	ft_analisis_comands2(t_pipex *pipex, t_minishell *shell, t_token *tmp)
{
	char	*temp;
	char	**split;

	if (tmp->str[0] == '/')
	{
		split = ft_split(tmp->str, ' ');
		temp = ft_strrchr(tmp->str, '/');
		pipex->command = split;
		return ;
	}
	pipex->command = ft_split(tmp->str, ' ');
	if (ft_path(&shell->env, &pipex, pipex->command[0]) == 0)
	{
		write(2, "Error finding path\n", 19);
		ft_free_pipex(&pipex);
		exit(127);
	}
}

t_token	*ft_analisis_comands(t_pipex *pipex, t_minishell *shell, t_token **save)
{
	t_token	*tmp;

	tmp = *save;
	if (ft_mega_if(save) == 1)
	{
		if (ft_strncmp("cd", (*save)->str, 2) == 0)
			return (tmp);
		while (*save && (*save)->type != PIPE && (*save)->type != AND
			&& (*save)->type != OR)
			*save = (*save)->next;
		if (*save && (*save)->type == PIPE)
			pipe(pipex->pipe[1]);
		if (!ft_check_heredoc(tmp, pipex, shell))
			return (NULL);
		pipex->pid = fork();
		pipex->childs++;
		if (pipex->pid && pipex->childs)
			ft_normal_signals();
		if (pipex->pid == 0 && tmp->type == COMMAND)
			ft_analisis_comands2(pipex, shell, tmp);
		if (tmp->type != HEREDOC)
			*save = tmp->next;
	}
	return (tmp);
}

t_token	*ft_analisis_redir(t_token *save, t_pipex *pipex, t_token *tmp)
{
	int	temp;

	if (pipex->childs != 0)
	{
		if (save->type == REDIR_IN)
			save = ft_redir(save, REDIR_IN, pipex);
		if (save && save->type == REDIR_OUT)
			save = ft_redir(save, REDIR_OUT, pipex);
		if (save && save->type == HEREDOC)
			save = save->next;
		if (save && save->type == APPEND)
			save = ft_redir(save, APPEND, pipex);
		if (save && save->type == PIPE)
			return (save);
	}
	else if (!tmp)
	{
		if (save && save->type == REDIR_OUT)
			temp = open(save->str, O_CREAT, 0777);
		else if (save && save->type == APPEND)
			temp = open(save->str, O_CREAT, 0777);
		save = save->next;
	}
	return (save);
}
