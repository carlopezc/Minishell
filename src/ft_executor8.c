/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor8.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 18:16:20 by lbellmas          #+#    #+#             */
/*   Updated: 2025/07/24 19:29:05 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../get_next_line/get_next_line_bonus.h"
#include "../header/ft_minishell.h"
#include <fcntl.h>
#include <sys/wait.h>

void	ft_pre_exec_command2(t_pipex *pipex,
		char **path_command, char ***command)
{
	char	*temp;

	if (!pipex->command)
	{
		write (2, "Command not found\n", 18);
		return ;
	}
	temp = ft_strjoin("/", pipex->command[0]);
	*path_command = ft_strjoin(pipex->path, temp);
	ft_safe_free((void **)&temp);
	*command = pipex->command;
}

void	ft_pre_exec_command(t_pipex *pipex, t_token *cmd, t_minishell *shell)
{
	char	*path_command;
	char	**command;
	char	**env;

	env = NULL;
	path_command = NULL;
	command = NULL;
	if (cmd->type == EXEC)
	{
		command = ft_split(cmd->str, ' ');
		path_command = command[0];
	}
	else
		ft_pre_exec_command2(pipex, &path_command, &command);
	if (pipex->path)
		ft_safe_free((void **)&pipex->path);
	env = ft_create_array_env(shell->env);
	execve(path_command, command, env);
	ft_free_array(env);
	ft_safe_free((void **)&path_command);
	exit(126);
}

void	ft_join_docs(int fd)
{
	char	*str;

	str = get_next_line(fd);
	while (str)
	{
		ft_printf("%s", str);
		free(str);
		str = get_next_line(fd);
	}
	close(fd);
}

void	ft_docs_in_child(t_pipex *pipex)
{
	int	p;

	if (pipex->pipe[0][0])
	{
		close(pipex->pipe[0][1]);
		ft_join_docs(pipex->pipe[0][0]);
	}
	if (pipex->heredoc)
	{
		ft_join_docs(pipex->heredoc);
		unlink(".heredoc");
	}
	p = 0;
	while (pipex->docs_in && pipex->docs_in[p])
	{
		ft_join_docs(pipex->docs_in[p]);
		p++;
	}
	exit(0);
}

int	ft_check_docs(int *docs)
{
	int	p;

	p = 0;
	while (docs[p])
	{
		if (docs[p] == -1)
		{
			perror(NULL);
			return (-1);
		}
		p++;
	}
	return (0);
}
/*
void	ft_docs_in(t_pipex *pipex)
{
	int		tmp_pipe[2];

	pipe(tmp_pipe);
	if (pipex->docs_in && ft_check_docs(pipex->docs_in) == -1)
		exit (1);
	pipex->pid = fork();
	if (pipex->pid == 0)
	{
		close(tmp_pipe[0]);
		dup2(tmp_pipe[1], 1);
		close(tmp_pipe[1]);
		ft_docs_in_child(pipex);
	}
	else
	{
		close(tmp_pipe[1]);
		close(pipex->pipe[0][0]);
		close(pipex->pipe[0][1]);
		waitpid(pipex->pid, NULL, 0);
		dup2(tmp_pipe[0], 0);
		close(tmp_pipe[0]);
	}
}*/
