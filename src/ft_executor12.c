/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor12.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 18:19:49 by lbellmas          #+#    #+#             */
/*   Updated: 2025/07/23 17:45:00 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../get_next_line/get_next_line_bonus.h"
#include "../header/ft_minishell.h"
#include <fcntl.h>
#include <sys/wait.h>

t_pipex	*ft_init_pipex(void)
{
	t_pipex	*pipex;

	pipex = (t_pipex *)malloc(sizeof(t_pipex));
	if (!pipex)
		return (NULL);
	pipex->childs = 0;
	pipex->docs_in = NULL;
	pipex->docs_out = NULL;
	pipex->command = NULL;
	pipex->path = NULL;
	pipex->pid = 0;
	pipex->brackets_count = 0;
	pipex->heredoc = 0;
	pipex->pipe[0][0] = 0;
	pipex->pipe[0][1] = 0;
	pipex->pipe[1][0] = 0;
	pipex->pipe[1][1] = 0;
	return (pipex);
}

t_token	*ft_heredoc(t_token *save, t_pipex *pipex)
{
	char	*str;
	int		temp;

	open(".heredoc", O_CREAT, 0777);
	temp = open(".heredoc", O_WRONLY);
	while (save && save->type == HEREDOC)
	{
		str = get_next_line(0);
		while (str && !(ft_strncmp(str, save->str, ft_strlen(save->str)) == 0
				&& str[ft_strlen(save->str)] == '\n'))
		{
			if (write(temp, str, ft_strlen(str)) == -1)
				return (save->next);
			free(str);
			str = get_next_line(0);
		}
		if (str)
			free(str);
		save = save->next;
	}
	close(temp);
	pipex->heredoc = open(".heredoc", O_RDONLY);
	return (save);
}

static int	ft_mega_if(t_token **save)
{
	if ((*save)->type == COMMAND || (*save)->type == EXEC || (*save)->type == HEREDOC
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

	if (ft_strchr(tmp->str, '/'))
	{
		temp = ft_strrchr(tmp->str, '/');
		pipex->command = ft_split(temp + 1, ' ');
		*temp = '\0';
		pipex->path = ft_strdup(tmp->str);
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
		pipex->pid = fork();
		pipex->childs++;
		if (pipex->pid == 0)
			ft_manage_child_signals();
		if (pipex->pid == 0 && tmp->type == COMMAND)
			ft_analisis_comands2(pipex, shell, tmp);
		if (tmp->type != HEREDOC)
			*save = tmp->next;
		else
			*save = tmp;
	}
	return (tmp);
}

t_token	*ft_analisis_redir(t_token *save, t_pipex *pipex)
{
	if (pipex->childs != 0)
	{
		if (save->type == REDIR_IN)
			save = ft_redir(save, REDIR_IN, pipex);
		if (save && save->type == REDIR_OUT)
			save = ft_redir(save, REDIR_OUT, pipex);
		if (save && save->type == HEREDOC)
			save = ft_heredoc(save, pipex);
		if (save && save->type == APPEND)
			save = ft_redir(save, APPEND, pipex);
		if (save && save->type == PIPE)
			return (save);
	}
	else
		save = save->next;
	return (save);
}
