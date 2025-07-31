/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 19:57:09 by lbellmas          #+#    #+#             */
/*   Updated: 2025/07/31 19:19:03 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_check_line(char *line, t_minishell *shell)
{
	if (!line)
		return (ft_manage_shell_signals(), 0);
	if (g_control_c == -1)
	{
		g_control_c = 0;
		unlink(".heredoc");
		shell->status = 130;
		return (free(line), ft_manage_shell_signals(), 0);
	}
	return (1);
}

int	ft_heredoc_loop(int fd, char *line, t_token **save, t_minishell *shell)
{
	line = NULL;
	line = readline("> ");
	if (ft_check_line(line, shell) == 0)
		return (0);
	while (ft_strncmp(line, (*save)->str, ft_strlen((*save)->str)))
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = readline("> ");
		if (ft_check_line(line, shell) == 0)
			return (0);
	}
	if (line)
	{
		free(line);
		line = NULL;
	}
	*save = (*save)->next;
	return (1);
}

t_token	*ft_heredoc(t_token *save, t_pipex *pipex, t_minishell *shell)
{
	int		fd;
	char	*line;
	t_token	*cpy;
	t_token	*prev;

	ft_manage_here_signals();
	cpy = save;
	fd = open(".heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	line = NULL;
	while (cpy && cpy->type == HEREDOC)
	{
		prev = cpy;
		if (ft_heredoc_loop(fd, line, &cpy, shell) == 0)
			return (close(fd), NULL);
	}
	if (line)
		free(line);
	close(fd);
	pipex->heredoc = open(".heredoc", O_RDONLY);
	ft_manage_shell_signals();
	if (!cpy)
		cpy = prev;
	return (cpy);
}

void	ft_here_signals(int sig)
{
	(void)sig;
	g_control_c = -1;
	write(1, "\n", 1);
}

void	ft_manage_here_signals(void)
{
	signal(SIGINT, ft_here_signals);
	signal(SIGQUIT, SIG_IGN);
}
