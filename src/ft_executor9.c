/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor9.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 18:17:06 by lbellmas          #+#    #+#             */
/*   Updated: 2025/07/23 23:58:32 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../get_next_line/get_next_line_bonus.h"
#include "../header/ft_minishell.h"
#include <fcntl.h>
#include <sys/wait.h>

static char	*ft_get_docs_out2(int tmp_pipe[2])
{
	char	*str;
	char	*trash;

	trash = NULL;
	str = get_next_line(tmp_pipe[0]);
	while (str)
	{
		trash = ft_strjoin(trash, str);
		free(str);
		str = get_next_line(tmp_pipe[0]);
	}
	return (trash);
}

static void	ft_get_docs_out(t_pipex *pipex, int tmp_pipe[2])
{
	char	*trash;
	int		p;

	p = 0;
	trash = ft_get_docs_out2(tmp_pipe);
	if (pipex->pipe[1][1])
	{
		close(pipex->pipe[1][0]);
		dup2(pipex->pipe[1][1], 1);
		close(pipex->pipe[1][1]);
		ft_printf("%s", trash);
	}
	while (pipex->docs_out[p])
	{
		dup2(pipex->docs_out[p], 1);
		close(pipex->docs_out[p]);
		ft_printf("%s", trash);
		p++;
	}
	free(trash);
	exit(0);
}

void	ft_docs_out(t_pipex *pipex)
{
	int	tmp_pipe[2];

	pipe(tmp_pipe);
	pipex->pid = fork();
	if (pipex->pid == 0)
	{
		close(tmp_pipe[0]);
		dup2(tmp_pipe[1], 1);
		close(tmp_pipe[1]);
	}
	else
	{
		close(tmp_pipe[1]);
		waitpid(pipex->pid, NULL, 0);
		ft_get_docs_out(pipex, tmp_pipe);
	}
}

void	ft_docs_out_true(t_pipex *pipex)
{
	if (ft_check_docs(pipex->docs_out) == -1)
		exit (1);
	if (pipex->docs_out[1] == 0 && pipex->pipe[1][1] == 0)
	{
		dup2(pipex->docs_out[0], 1);
		close(pipex->docs_out[0]);
	}
	else
		ft_docs_out(pipex);
}

void	ft_heredoc_lonely(t_pipex *pipex)
{
	char	*str;

	str = get_next_line(0);
	while (str)
	{
		ft_printf("%s", str);
		free(str);
		str = NULL;
		str = get_next_line(0);
	}
	ft_free_pipex(&pipex);
	exit(0);
}
