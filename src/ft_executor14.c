/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor14.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 23:12:59 by lbellmas          #+#    #+#             */
/*   Updated: 2025/07/24 20:13:55 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_decide_exec(t_minishell *shell, t_token *save, t_pipex *pipex)
{
	if (save->type == HEREDOC)
		exit(0);
	if (save->type == BUILTIN)
	{
		ft_exec_build(shell, save->str);
		if (pipex->childs != 0)
			exit (0);
	}
	else
	{
		ft_manage_child_signals();
		ft_pre_exec_command(pipex, save, shell);
	}
}

void	ft_exec(t_minishell *shell, t_pipex *pipex, t_token *save)
{
	if (pipex->pipe[0][0] && !pipex->docs_in && !pipex->heredoc)
	{
		close(pipex->pipe[0][1]);
		dup2(pipex->pipe[0][0], 0);
		close(pipex->pipe[0][0]);
	}
	else if (pipex->docs_in || pipex->pipe[0][0] || pipex->heredoc)
		ft_docs_in(pipex);
	if (pipex->docs_out)
		ft_docs_out_true(pipex);
	else if (pipex->pipe[1][1])
	{
		close(pipex->pipe[1][0]);
		dup2(pipex->pipe[1][1], 1);
		close(pipex->pipe[1][1]);
	}
	if (save->type == HEREDOC && (pipex->docs_out || pipex->pipe[1][1]))
		ft_heredoc_lonely(pipex);
	ft_decide_exec(shell, save, pipex);
}

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
}

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

void	ft_cd_route(t_minishell *shell, char *cmd)
{
	t_env	*pwd;
	t_env	*node;
	char	*tmp;

	pwd = shell->env;
	while (pwd && pwd->next && ft_strncmp((pwd->next)->name,
			"PWD", ft_max_strlen("PWD", (pwd->next)->name)))
		pwd = pwd->next;
	if (!pwd)
		return ;
	if (access(cmd + 3, F_OK) == 0 || ft_strncmp("/", cmd + 3, 2) == 0)
	{
		ft_old_pwd(&shell->env, pwd->next);
		tmp = ft_strdup(cmd + 3);
		while (ft_strnstr(tmp, "../", ft_strlen(tmp)))
			tmp = ft_correct_cd(tmp);
		if (ft_strnstr(tmp, "..", ft_strlen(tmp)))
			tmp = ft_errase_route(tmp);
		node = ft_create_node(ft_strdup("PWD"), tmp);
		ft_add_node(&shell->env, pwd, node);
		chdir(cmd + 3);
		return ;
	}
	write (2, "No such file or directory\n", 26);
}
