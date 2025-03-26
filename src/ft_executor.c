/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:37:21 by lbellmas          #+#    #+#             */
/*   Updated: 2025/03/26 15:31:38 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_arrange_fd(t_pipex *pipex)
{
	if (pipex->pipe[0][0] != 0)
	{
		close(pipex->pipe[0][0]);
		close(pipex->pipe[0][0]);
		pipex->pipe[0][0] = 0;
		pipex->pipe[0][0] = 0;
	}
	if (pipex->pipe[1][0] != 0)
	{
		pipex->pipe[0][0] = pipex->pipe[1][0];
		pipex->pipe[0][1] = pipex->pipe[0][1];
		pipex->pipe[1][0] = 0;
		pipex->pipe[1][1] = 0;
	}
	if (pipex->docs[0] != 0)
	{
		close(pipex->docs[0]);
		pipex->docs[0] = 0;
	}
	if (pipex->docs[1] != 0)
	{
		close(pipex->docs[1]);
		pipex->docs[1] = 0;
	}
}

static char	**ft_free_executor(t_pipex **pipex)
{
	char	**return_v;

	if ((*pipex)->path)
		free((*pipex)->path);
	return_v = (*pipex)->command;
	free(*pipex);
	*pipex = NULL;
	return (return_v);
}

void	ft_exec_command(t_pipex *pipex, char **env)
{
	char	*temp;
	char	*path_command;
	char	*command;

	temp = ft_strjoin("/", pipex->command[0]);
	path_command = ft_strjoin(pipex->path, temp);
	free(temp);
	command = ft_free_exec_cmd(pipex);
	execve(path_command, command, env);
}

void	ft_check_cd(char *temp, char *pwd)
{
	char	temp;
	char	*final;

	temp = ft_strjoin(pwd, "/");
	final = ft_strjoin(temp, temp);
	free(temp);
	if (acces(final, F_OK) != 0)
		return (free(final), 0);
	else
		return (free(final), 1);
}

void	ft_cd(t_minishell *shell, char *cmd)
{
	char	*temp;
	char	*join;
	int	p;
	int	u;

	if (ft_strlen(cmd) == 2)
	{
		while (shell->env[p] && ft_strncmp(env[p], "PWD=", 4) != 0)
			p++;
		if (!shell->env[p])
			return ;
		free(shell->env[p]);
		u = 0;
		while (shell->env[u] && ft_strncmp(env[u], "USER=", 5))
			u++;
		shell->env[p] = ft_strjoin("/home", shell->env[u] + 5);
		return ;
	}
	if (cmd[3] == '/')
	{
		while (shell->env[p] && ft_strncmp(env[p], "PWD=", 4) != 0)
			p++;
		if (!shell->env[p])
			return ;
		if (access(cmd + 3, F_OK) != 0)
		{
			free(shell->env[p]);
			env[p] = ft_strjoin(cmd + 3, "\n");
			return ;
		}
		else
			return ;
	}
	if (ft_strncmp(ft_strchr(cmd, " ") + 1, "..", 2) == 0)
	{
		ft_errase_pwd(shell);
		temp = ft_strchr(cmd, "/");
		while (temp && ft_strncmp(temp + 1), "..")
		{
			ft_errase_pwd(shell);
			temp = ft_strchr(temp + 1, "/");
		}
	}
	else
		temp = ft_strchr(cmd, " ");
	if (temp)
	{
		p = 0;
		while (shell->env[p] && ft_strncmp(env[p], "PWD=", 4) != 0)
			p++;
		if (!shell->env[p])
			return ;
		while (temp)
		{
			if (ft_check_cd(temp + 1, shell->env[p] + 4) == 0)
				return ;
			join = ft_strjoin(shell->env[p], "/");
			free(shell->env[p]);
			shell->env[p] = t_strjoin(join, temp + 1);
			temp = ft_strchr(temp + 1, "/");
		}
	}
	temp = ft_strdup(shell->env[p]);
	free(shell->env[p]);
	shell->env[p] = ft_strjoin(temp, "\n");
	free(temp);
}

void	ft_pwd(t_minishell *shell)
{
	int	p;

	p = 0;
	while (shell->env[p] && ft_strncmp(env[p], "PWD=", 4) != 0)
		p++;
	ft_printf("%s", ft_strchr(shell->env[p], "/"));
}

void	ft_env(t_minishell *shell)
{
	int	p;

	p = 0;
	while (shell->env[p])
	{
		ft_printf("%s", shell->env[p]);
		p++;
	}
}

void	ft_echo(char *cmd)
{
	char	*temp;

	temp = ft_strchr(cmd, " ");
	if (ft_strncmp(temp + 1, "-n", 2) == 0)
	{
		temp = ft_strchr(temp, " ");
		write(1, temp + 1, ft_strlen(temp + 1) - 1);
	}
	else
		ft_printf("%s", temp + 1);
}

void	ft_exec_build(t_minishell *shell, char *cmd)
{
	if (ft_strncmp(shell->tokens[p]->str, "echo", 4) == 0)
		ft_echo(cmd);
	if (ft_strncmp(shell->tokens[p]->str, "cd", 2) == 0)
		ft_cd(shell, cmd);
	if (ft_strncmp(shell->tokens[p]->str, "pwd", 3) == 0)
		ft_pwd(shell);
	if (ft_strncmp(shell->tokens[p]->str, "export", 6) == 0)
		ft_export();
	if (ft_strncmp(shell->tokens[p]->str, "unset", 5) == 0)
		ft_unset();
	if (ft_strncmp(shell->tokens[p]->str, "env", 3) == 0)
		ft_env(shell);
	if (ft_strncmp(shell->tokens[p]->str, "exit", 4) == 0)
		ft_exit();
}

void	ft_exec_command(t_pipex *pipex, char **env, t_token *cmd)
{
	char	*temp;
	char	*path_command;
	char	**command;

	if (cmd->type == ejecutable)
	{
		command = ft_split(cmd->str);
		path_command = command[0];
	}
	else
	{
		temp = ft_strjoin("/", pipex->command[0]);
		path_command = ft_strjoin(pipex->path, temp);
		free(temp);
		command = pipex->command;
	}
	free(cmd->str);
	free(cmd);
	if (pipex->path)
		free(pipex->path);
	free(pipex);
	while (*env)
		free(*env++);
	free(env);
	execve(path_command, command, env);
}

void	ft_exec(t_minishell *shell, t_pipex *pipex, int p)
{
	if (shell->tokens[p]->type == build)
	{
		ft_exec_build(shell, shell->tokens[p]->str);
	}
	if (shell->tokens[p]->type == command || shell->tokens[p]->type == ejecutable)
	{
		if (pipex->docs[0] != 0)
		{
			dup2(pipex->docs[0], 0);
			close(pipex->docs[0]);
		}
		else
		{
			dup2(pipex->pipe[0][0], 0);
			close(pipex->pipe[0][0]);
		}
		if (pipex->docs[1] != 0)
		{
			dup2(pipex->docs[1], 1);
			close(pipex->docs[1]);
		}
		else
		{
			dup2(pipex->pipe[1], 1);
			close(pipex->pipe[1]);
		}
		ft_exec_command(pipex, shell->env, shell->tokens[p]);
	}
}

char	*ft_find_path(char **env)
{
	int	p;

	p = 0;
	while (env[p])
	{
		if (ft_strncmp(env[p], "PATH=/", 6) == 0)
			return (env[p] + 6);
		p++;
	}
	return (NULL);
}

static int	ft_good_path(char *path, t_pipex **pipex, char **split, int p)
{
	free(path);
	(*pipex)->path = ft_strdup(split[p]);
	return (ft_clear_split(split), 1);
}

int	ft_path(char **env, t_pipex **pipex, char *cmd)
{
	char	**split;
	char	*temp;
	char	*path;
	int		p;

	if (!env || !*env)
		return (0);
	if ((*pipex)->path)
		free((*pipex)->path);
	split = ft_split(ft_find_path(env), ':');
	if (!split)
		return (0);
	p = 0;
	while (split[p])
	{
		temp = ft_strjoin("/", cmd);
		path = ft_strjoin(split[p], temp);
		free(temp);
		if (access(path, X_OK) == 0)
			return (ft_good_path(path, pipex, split, p));
		free(path);
		p++;
	}
	(*pipex)->path = NULL;
	return (ft_clear_split(split), 0);
}

int	ft_executor(t_minishell *shell)
{
	int	p;
	t_pipex	*pipex;

	p = 0;
	while (shell[p])
	{
		while (shell->tokens[p++]->type != pipe)
		{
			if (shell->tokens[p]->type == command)
			{
				ft_path(shell->env, pipex, shell->tokens[p]->str);
				pipex->pid = fork();
				p++;
			}
			if (pipex->pid == 0 && shell->tokens[p++]->type == redirect_infile)
				pipex->docs[0] = open(shell->tokens[p++]->type, O_RDONLY);
			if (pipex->pid == 0 && shell->tokens[p++]->type == redirect_outfile)
				pipex->docs[1] = open(shell->tokens[p++]->type, O_WRONLY);
			else if (shell->tokens[p] && pipe(pipex->pipe[1]))
				return (-1);
			if (pipex->docs[0] < 0)
				pipex->docs[0] = 0;
			if (pipex->docs[1] < 0)
				pipex->docs[1] = 0;
			if (pipex->pid == 0)
				ft_exec(shell, pipex, p);
			else
			{
				if (shell->tokens[p]->type == redirect_infile)
					p++;
				if (shell->tokens[p]->type == redirect_outfile)
					p++;
			}
		}
		p++;
		ft_arrange_fd(pipex);
	}
}
