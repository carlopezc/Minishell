/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:37:21 by lbellmas          #+#    #+#             */
/*   Updated: 2025/04/09 18:37:48 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"
#include <fcntl.h>
#include <sys/wait.h>

void	ft_safe_free(void **p)
{
	if (p && *p)
	{
		//ft_printf("Liberando puntero : %p\n", *p);
		free(*p);
		*p  = NULL;
	}
	return ;
}

void	ft_arrange_fd(t_pipex *pipex)
{
	if (!pipex || !pipex->pipe)
		return ;
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
	if (pipex->docs_in)
		ft_safe_free((void **)&pipex->docs_in);
	if (pipex->docs_out)
		ft_safe_free((void **)&pipex->docs_out);
}

static char	**ft_free_exec_cmd(t_pipex **pipex)
{
	char	**return_v;

	if ((*pipex)->path)
	{
		//ft_printf("ft_free_exec_cmd: %p\n", (*pipex)->path);
		ft_safe_free((void **)&(*pipex)->path);
	}
	return_v = (*pipex)->command;
	ft_safe_free((void **)pipex);
	return (return_v);
}

void	ft_exec_command(t_pipex *pipex, char **env)
{
	char	*temp;
	char	*path_command;
	char	**command;

	temp = ft_strjoin("/", pipex->command[0]);
	path_command = ft_strjoin(pipex->path, temp);
	//ft_printf("ft_exec_command: %p\n", temp);
	ft_safe_free((void **)&temp);
	command = ft_free_exec_cmd(&pipex);
	execve(path_command, command, env);
}

int	ft_check_cd(char *file, char *pwd)
{
	char	*temp;
	char	*final;

	temp = ft_strjoin(pwd, "/");
	final = ft_strjoin(temp, file);
	ft_safe_free((void **)&temp);
	if (access(final, F_OK) != 0)
		return (ft_safe_free((void **)&final), 0);
	else
		return (ft_safe_free((void **)&final), 1);
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
		ft_safe_free((void **)&shell->env[p]);
		u = 0;
		while ((u == p) || (shell->env[u] && ft_strncmp(shell->env[u], "HOME=", 5) != 0))
			u++;
		if (!shell->env[u])
			return ;
		shell->env[p] = ft_strjoin("PWD=", shell->env[u] + 5);
		return ;
	}
	if (cmd[3] == '/')
	{
		while (shell->env[p] && ft_strncmp(shell->env[p], "PWD=", 4) != 0)
			p++;
		if (!shell->env[p])
			return ;
		if (access(cmd + 3, F_OK) == 0)
		{
			ft_safe_free((void **)&shell->env[p]);
			shell->env[p] = ft_strjoin("PWD=", cmd + 3);
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
		if (ft_check_cd(temp + 1, shell->env[p] + 4) == 0)
			return ;
		join = ft_strjoin(shell->env[p] + 4, "/");
		ft_safe_free((void **)&shell->env[p]);
		shell->env[p] = ft_strjoin(join, temp + 1);
		temp = shell->env[p];
		shell->env[p] = ft_strjoin("PWD=", temp);
		ft_safe_free((void **)&temp);
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
	if (!shell->env[p])
		ft_printf("te cagas\n");
	ft_printf("%s\n", ft_strchr(shell->env[p], '/'));
	return ;
}

void	ft_print_env(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
		ft_printf("%s\n", env[i++]);
	return ;
}

void	ft_env(t_minishell *shell, char *cmd)
{
	//esta funcion esta mal jeje
 	int	i;
 	int flag;
 	char	**env;
 
 	i = 3;
 	flag = 0;
 	env = shell->env;
 	// Falta mirar si los caracteres son validos para nombre de variable
 	while (cmd[i])
 	{
 		if (cmd[i] == '=')
 		{
 			//ft_printf("Entra\n");
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
 	if (flag)
 		env = shell->env_temporal;
	ft_print_env(env);
	return ;
 }

void	ft_print_export(t_minishell *shell)
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

void	ft_add_to_export(char *cmd, t_minishell **shell)
{
	char	**export;
	char	**cpy;
	int	i;

	export = (*shell)->export;
	if (!cmd || !export || !*export)
		return ;
	i = ft_arraylen(export);
	cpy = (char **)malloc((i + 2) * sizeof(char *));
	if (!cpy)
		return ;
	i = 0;
	while (export[i])
	{
		cpy[i] = ft_strdup(export[i]);
		if (!cpy[i])
		{
			ft_free_array(cpy);
			cpy = NULL;
			return ;
		}
		i++;
	}
	cpy[i++] = cmd;
	cpy[i] = NULL;
	(*shell)->export = cpy;
	return ;
}

void	ft_export(t_minishell *shell, char *cmd)
{
	int	i;

	i = 6;
	if (!ft_strncmp(cmd, "export", ft_strlen(cmd)))
		return (ft_print_export(shell));
	while (cmd[i])
	{
		if (cmd[i] == '=')
		{
			ft_add_to_env(&shell, cmd + 7, 0);
			break ;
		}
		i++;
	}
	if (shell->export)
	{
		ft_free_array(shell->export);
		shell->export = NULL;
	}
	if (!cmd[i])
		ft_add_to_export(cmd + 7, &shell);
	else
		shell->export = ft_create_export(ft_strdup_env(shell->env));
	//si hay arg no deberia imprimir solo para probar
	ft_print_export(shell);
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
	return ;
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
		ft_export(shell, cmd);
	if (ft_strncmp(cmd, "unset", 5) == 0)
		return ;
		//ft_unset();
	if (ft_strncmp(cmd, "env", 3) == 0)
		ft_env(shell, cmd);
	if (ft_strncmp(cmd, "exit", 4) == 0)
	{
		ft_free_minishell(&shell);
		exit(0) ;
	}
	return ;
	//ft_free_minishell(&shell);
	//exit(0);
}

void	ft_pre_exec_command(t_pipex *pipex, char **env, t_token *cmd, t_minishell *shell)
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
		//ft_printf("ft_pre_exec_command: %p\n", temp);
		ft_safe_free((void **)&temp);
		//ft_printf("%s\n", path_command);
		command = pipex->command;
	}
	if (pipex->path)
	{
		//ft_printf("ft_pre_exec_command: %p\n", pipex->path);
		ft_safe_free((void **)&pipex->path);
	}
	ft_free_minishell(&shell);
	execve(path_command, command, env);
}

void	ft_exec(t_minishell *shell, t_pipex *pipex, t_token *save)
{
	if (save->type == BUILTIN)
		ft_exec_build(shell, save->str);
	if (save->type == COMMAND || save->type == EXEC)
	{
		if (pipex->docs_in)
		{
			dup2(pipex->docs_in[0], 0);
			close(pipex->docs_in[0]);
		}
		else if (pipex->pipe[0][0])
		{
			//ft_printf("lee pipe %i\n", pipex->pipe[0][0]);
			close(pipex->pipe[0][1]);
			dup2(pipex->pipe[0][0], 0);
			close(pipex->pipe[0][0]);
		}
		if (pipex->docs_out)
		{
			dup2(pipex->docs_out[0], 1);
			close(pipex->docs_out[0]);
		}
		else if (pipex->pipe[1][1])
		{
			//ft_printf("ecribe pipe %i\n", pipex->pipe[1][1]);
			close(pipex->pipe[1][0]);
			dup2(pipex->pipe[1][1], 1);
			close(pipex->pipe[1][1]);
		}
		char **env = ft_strdup_env(shell->env);
		ft_pre_exec_command(pipex, env, save, shell);
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
	(void)path;
	(*pipex)->path = ft_strdup(split[p]);
	//ft_free_array(split);
	//split = NULL;
	return (1);
}

int	ft_path(char **env, t_pipex **pipex, char *cmd)
{
	char	**split;
	char	*temp;
	char	*path;
	int		p;
	int	res;

	if (!env || !*env)
		return (0);
	if ((*pipex)->path)
		ft_safe_free((void **)&(*pipex)->path);
	split = ft_split(ft_find_path(env), ':');
	if (!split)
		return (0);
	p = 0;
	while (split[p])
	{
		temp = ft_strjoin("/", cmd);
		path = ft_strjoin(split[p], temp);
		ft_safe_free((void **)&temp);
		if (access(path, X_OK) == 0)
		{
			res = ft_good_path(path, pipex, split, p);
			ft_free_array(split);
			ft_safe_free((void **)&path);
			split = NULL;
			return (res);
		}
		ft_safe_free((void **)&path);
		p++;
	}
	(*pipex)->path = NULL;
	ft_free_array(split);
	split = NULL;
	return (0);
}

void	ft_free_pipex(t_pipex *pipex)
{
	int	p;

	p = 0;
	if (!pipex)
		return ;
	if (pipex->path)
		ft_safe_free((void **)&pipex->path);
	if (pipex->command)
	{
		while (pipex->command[p])
		{
			ft_safe_free((void **)&pipex->command[p]);
			p++;
		}
		ft_safe_free((void **)&pipex->command);
	}
	pipex->path = NULL;
	pipex->command = NULL;
	//free(pipex);
}

t_token *ft_redir(t_token *save, t_token_type type, t_pipex *pipex)
{
	t_token *temp;
	int	count;

	temp = save;
	count = 0;
	while (temp->type == type)
	{
		count++;
		temp = temp->next;
	}
	pipex->docs_in = (int *)malloc(count * sizeof(int));
	count = 0;
	while (save->type == type)
	{
		if (type == REDIR_IN)
			pipex->docs_in[count] = open(save->str, O_RDONLY);
		else
			pipex->docs_out[count] = open(save->str, O_WRONLY);
		save = save->next;
	}
	return (save);
}

void	ft_init_pipex(t_pipex **pipex)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	(*pipex)->childs = 0;
	(*pipex)->docs_in = NULL;
	(*pipex)->docs_out = NULL;
	while ((*pipex)->pipe[i])
	{
		while ((*pipex)->pipe[i][j])
			(*pipex)->pipe[i][j++] = 0;
		i++;
		j = 0;
	}
	(*pipex)->pid = 0;
	(*pipex)->command = NULL;
	(*pipex)->path = NULL;
	return ;
}

int	ft_executor(t_minishell *shell)
{
	t_token	*save;
	t_token	*tmp;
	t_pipex	*pipex = (t_pipex *)malloc(sizeof(t_pipex));
	if (!pipex)
		return (-1);
	//ft_init_pipex(&pipex);
	pipex->childs = 0;
	pipex->docs_in = NULL;
	pipex->docs_out = NULL;
	pipex->path = NULL;
	save = shell->tokens;
	while (save)
	{
		while (save && save->type != AND && save->type != OR)
		{
			if (save->type == COMMAND || save->type == BUILTIN || save->type == EXEC)
			{
				tmp = save;
				pipex->pid = fork();
				if (pipex->pid != 0)
					pipex->childs++;
				if (pipex->pid == 0 && save->type == COMMAND)
				{
					pipex->command = ft_split(save->str, ' ');
					if (ft_path(shell->env, &pipex, pipex->command[0]) == 0)
						return (-1);
				}
			}
			while (save && (save->type == REDIR_IN || save->type == REDIR_OUT || save->type == PIPE))
			{
				if (save->type == REDIR_IN)
					save = ft_redir(save, REDIR_IN, pipex);
				if (save->type == REDIR_OUT)
					save = ft_redir(save, REDIR_OUT, pipex);
				if (save->type == PIPE)
				{
					pipe(pipex->pipe[1]);
					break ;
				}
			}
			if (pipex->pid == 0)
			{
				ft_exec(shell, pipex, tmp);
				save = save->next;
			}
			else
			{
				while (save && save->type != PIPE && save->type != AND && save->type != OR)
					save = save->next;
			}
		}
		while (pipex->childs > 0)
		{
			wait(NULL);
			pipex->childs--;
		}
		ft_arrange_fd(pipex);
	}
	return (0);
}
