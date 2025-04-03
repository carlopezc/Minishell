/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:37:21 by lbellmas          #+#    #+#             */
/*   Updated: 2025/04/03 13:35:34 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"
#include <fcntl.h>
#include <sys/wait.h>

void	ft_arrange_fd(t_pipex *pipex)
{
	if (pipex->pipe[0][0] != 0)
	{
		close(pipex->pipe[0][0]);
		close(pipex->pipe[0][1]);
		pipex->pipe[0][0] = 0;
		pipex->pipe[0][1] = 0;
	}
	if (pipex->pipe[1][0] != 0)
	{
		pipex->pipe[0][0] = pipex->pipe[1][0];
		pipex->pipe[0][1] = pipex->pipe[1][1];
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

static char	**ft_free_exec_cmd(t_pipex **pipex)
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
	char	**command;

	temp = ft_strjoin("/", pipex->command[0]);
	path_command = ft_strjoin(pipex->path, temp);
	free(temp);
	command = ft_free_exec_cmd(&pipex);
	execve(path_command, command, env);
}

int	ft_check_cd(char *file, char *pwd)
{
	char	*temp;
	char	*final;

	temp = ft_strjoin(pwd, "/");
	final = ft_strjoin(temp, file);
	free(temp);
	if (access(final, F_OK) != 0)
		return (free(final), 0);
	else
		return (free(final), 1);
}

void	ft_errase_pwd(t_minishell *shell)
{
	int	p;
	int	c;

	p = 0;
	while (shell->env[p] && ft_strncmp(shell->env[p], "PWD=", 4) != 0)
		p++;
	if (!shell->env[p])
		return ;
	c = ft_strlen(shell->env[p]) - 1;
	while (shell->env[p][c] != '/')
		shell->env[p][c--] = '\0';
	if (shell->env[p][c] != '=' && shell->env[p][c -1] != '=')
		shell->env[p][c] = '\0';
}

void	ft_cd(t_minishell *shell, char *cmd)
{
	char	*temp;
	char	*join;
	int	p;
	int	u;


	p = 0;
	if (ft_strncmp(cmd, "cd", 3) == 0)
	{
		while (shell->env[p] && ft_strncmp(shell->env[p], "PWD=", 4) != 0)
			p++;
		if (!shell->env[p])
			return ;
		free(shell->env[p]);
		shell->env[p] = NULL;
		u = 0;
		while (u != p && shell->env[u] && ft_strncmp(shell->env[u], "HOME=", 5) != 0)
			u++;
		if (!shell->env[u])
			return ;
		shell->env[p] = ft_strdup(shell->env[u] + 5);
		return ;
	}
	if (cmd[3] == '/')
	{
		while (shell->env[p] && ft_strncmp(shell->env[p], "PWD=", 4) != 0)
			p++;
		if (!shell->env[p])
			return ;
		if (access(cmd + 3, F_OK) != 0)
		{
			free(shell->env[p]);
			shell->env[p] = ft_strjoin(cmd + 3, "\n");
			return ;
		}
		else
			return ;
	}
	if (ft_strncmp(ft_strchr(cmd, ' ') + 1, "..", 2) == 0)
	{
		ft_errase_pwd(shell);
		temp = ft_strchr(cmd, '/');
		while (temp && ft_strncmp(temp + 1, "..", 2) == 0)
		{
			ft_errase_pwd(shell);
			temp = ft_strchr(temp + 1, '/');
		}
	}
	else
		temp = ft_strchr(cmd, ' ');
	if (temp)
	{
		p = 0;
		while (shell->env[p] && ft_strncmp(shell->env[p], "PWD=", 4) != 0)
			p++;
		if (!shell->env[p])
			return ;
		while (temp)
		{
			if (ft_check_cd(temp + 1, shell->env[p] + 4) == 0)
				return ;
			join = ft_strjoin(shell->env[p] + 4, "/");
			free(shell->env[p]);
			shell->env[p] = ft_strjoin(join, temp + 1);
			temp = ft_strchr(temp + 1, '/');
		}
	}
	//shell->env[p] = ft_strdup(shell->env[p]);
	//free(shell->env[p]);
//	shell->env[p] = ft_strjoin(temp, "\n");
//	free(temp);
}

void	ft_pwd(t_minishell *shell)
{
	int	p;

	p = 0;
	while (shell->env[p] && ft_strncmp(shell->env[p], "PWD=", 4) != 0)
		p++;
	ft_printf("%s\n", ft_strchr(shell->env[p], '/'));
}

void	ft_env(t_minishell *shell, char *cmd)
{
	int	p = 0;
	int	i;
	int flag;
	char	**env;

	p = 0;
	i = 3;
	flag = 0;
	env = shell->env;
	// Falta mirar si los caracteres son validos para nombre de variable
	ft_printf("%s\n", cmd);
	while (cmd[i])
	{
		ft_printf("la letra es %c\n", cmd[i]);
		if (cmd[i] == '=')
		{
			ft_printf("Entra\n");
			//usare misma funcion cuando haga export, 1 para var temporal (en este caso), 0 para global
			ft_add_to_env(&shell, cmd + 4, 1);
			flag = 1;
			break ;
			//lo añade y ademas imprime el env
			//ademas no se añade al export porque es variable temporal
			//solo se añade para este readline, luego tiene que volver a antes !!! amai
		}
		i++;
	}
	ft_printf("Flag vale %i\n", flag);
	if (flag)
	{
		ft_printf("env temporal \n");
		env = shell->env_temporal;
	}
	//(void)cmd;
	while (env[p])
	{
		ft_printf("%s\n", env[p]);
		p++;
	}
}

void	ft_export(t_minishell *shell)
{
	int	p;

	p = 0;
	while (shell->export[p])
	{
		ft_printf("declare -x ");
		ft_printf("%s\n", shell->export[p]);
		p++;
	}
	return ;
}

void	ft_echo(char *cmd)
{
	char	*temp;

	temp = ft_strchr(cmd, ' ');
	if (ft_strncmp(temp + 1, "-n", 2) == 0)
	{
		ft_printf("%s", temp + 4);
	}
	else
		ft_printf("%s\n", temp + 1);
}

void	ft_exec_build(t_minishell *shell, char *cmd)
{
	if (ft_strncmp(cmd, "echo", 4) == 0)
		ft_echo(cmd);
	if (ft_strncmp(cmd, "cd", 2) == 0)
	{
		ft_cd(shell, cmd);
		ft_pwd(shell);
	}
	if (ft_strncmp(cmd, "pwd", 3) == 0)
		ft_pwd(shell);
	if (ft_strncmp(cmd, "export", 6) == 0)
		ft_export(shell);
	if (ft_strncmp(cmd, "unset", 5) == 0)
		return ;
		//ft_unset();
	if (ft_strncmp(cmd, "env", 3) == 0)
		ft_env(shell, cmd);
	if (ft_strncmp(cmd, "exit", 4) == 0)
		exit(0) ;
		//ft_exit();
}

void	ft_pre_exec_command(t_pipex *pipex, char **env, t_token *cmd)
{
	char	*temp;
	char	*path_command;
	char	**command;

	if (cmd->type == EXEC)
	{
		command = ft_split(cmd->str, ' ');
		path_command = command[0];
	}
	else
	{
		temp = ft_strjoin("/", pipex->command[0]);
		path_command = ft_strjoin(pipex->path, temp);
		free(temp);
		ft_printf("%s\n", path_command);
		command = pipex->command;
	}
//	if (pipex->path)
//		free(pipex->path);
//	free(pipex);
//	while (*env)
//		free(*env++);
//	free(env);
	execve(path_command, command, env);
}

void	ft_exec(t_minishell *shell, t_pipex *pipex, t_token *save)
{
	if (save->type == BUILTIN)
	{
		//printf("El save str es: %s\n", save->str);
		//return ;
		ft_exec_build(shell, save->str);
	}
	if (save->type == COMMAND || save->type == EXEC)
	{
		if (pipex->docs[0] != 0)
		{
			dup2(pipex->docs[0], 0);
			close(pipex->docs[0]);
		}
		else if (pipex->pipe[0][0])
		{
			ft_printf("lee pipe %i\n", pipex->pipe[0][0]);
			close(pipex->pipe[0][1]);
			dup2(pipex->pipe[0][0], 0);
			close(pipex->pipe[0][0]);
		}
		if (pipex->docs[1] != 0)
		{
			dup2(pipex->docs[1], 1);
			close(pipex->docs[1]);
		}
		else if (pipex->pipe[1][1])
		{
			ft_printf("ecribe pipe %i\n", pipex->pipe[1][0]);
			close(pipex->pipe[1][0]);
			dup2(pipex->pipe[1][1], 1);
			close(pipex->pipe[1][1]);
		}
		ft_pre_exec_command(pipex, shell->env, save);
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

void	ft_free_pipex(t_pipex *pipex)
{
	int	p;

	p = 0;
	if (!pipex)
		return ;
	if (pipex->path)
		free(pipex->path);
	if (pipex->command)
	{
		while (pipex->command[p])
		{
			free(pipex->command[p]);
			p++;
		}
		free(pipex->command);
	}
	pipex->path = NULL;
	pipex->command = NULL;
	//free(pipex);
}

int	ft_executor(t_minishell *shell)
{
	t_token	*tmp;
	t_token	*save;
	t_pipex	*pipex = (t_pipex *)malloc(sizeof(pipex));
	pipex->docs[0] = 0;
	pipex->docs[1] = 0;

	save = shell->tokens;
	while (save)
	{
		while (save && save->type != PIPE)
		{
			if (save && save->type == COMMAND)
			{
				pipex->command = ft_split(save->str, ' ');
				if (ft_path(shell->env, &pipex, pipex->command[0]) == 0)
					return (-1);
				pipex->pid = fork();
				if (pipex->pid > 0)
					waitpid(pipex->pid, NULL, 0);
				tmp = save;
				save = save->next;
			}
			if (save && (save->type == BUILTIN || save->type == EXEC))
			{
				pipex->pid = fork();
				if (pipex->pid > 0)
					waitpid(pipex->pid, NULL, 0);
				tmp = save;
				save = save->next;
			}
			if (pipex->pid == 0 && save && save->type == REDIR_IN)
			{
				pipex->docs[0] = open(save->str, O_RDONLY);
				save = save->next;
			}
			if (pipex->pid == 0 && save && save->type == REDIR_OUT)
			{
				pipex->docs[1] = open(save->str, O_WRONLY);
				save = save->next;
			}
			if (pipex->pid == 0 && save && save->type == PIPE)//&& pipe(pipex->pipe[1]) == -1)
			{
				ft_printf("adeu2\n");
				pipe(pipex->pipe[1]);
				//return (-1);
			}
			else if (save && pipe(pipex->pipe[1]))
				return (-1);
			if (pipex->docs[0] < 0)
				pipex->docs[0] = 0;
			if (pipex->docs[1] < 0)
				pipex->docs[1] = 0;
			if (pipex->pid == 0)
				ft_exec(shell, pipex, tmp);
			else
			{
				if (save && save->type == REDIR_IN)
					save = save->next;
				if (save && save->type == REDIR_OUT)
					save = save->next;
			}
		}
		if (save) //&& save->type != PIPE)
			save = save->next;
		ft_arrange_fd(pipex);
		ft_printf("fin del bucle\n");
	}
	return (1);
}
