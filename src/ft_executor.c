/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:37:21 by lbellmas          #+#    #+#             */
/*   Updated: 2025/04/19 17:02:32 by carlotalcd       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"
#include <fcntl.h>
#include <sys/wait.h>

void	ft_safe_free(void **p)
{
	if (p && *p)
	{
		free(*p);
		*p  = NULL;
	}
	return ;
}

void	ft_arrange_fd(t_pipex *pipex)
{
	if (!pipex)
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
/*
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
*/
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
	int	c;
	t_env *env;

	env = shell->env;
	while (env && ft_strncmp(env->name, "PWD", 3))
		env = env->next;
	if (!env)
		return ;
	c = ft_strlen(env->value) - 1;
	while (env->value[c] != '/')
		env->value[c--] = '\0';
	if (env->value[c] != '=' && env->value[c -1] != '=')
		env->value[c] = '\0';
}

void	ft_add_node(t_env **list, t_env *prev, t_env *node)
{
	t_env *tmp;
	t_env *next;

	next = NULL;
	tmp = *list;
	if (!tmp)
		return ;
	while (tmp)
	{
		if (tmp == prev)
		{
			next = (tmp->next)->next;
			ft_free_node(tmp->next, list);
			prev->next = node;
			node->next = next;
		}
		tmp = tmp->next;
	}
	return ;
}
void	ft_cd(t_minishell *shell, char *cmd)
{
	char	*temp;
	char	*join;

	t_env	*pwd;
	t_env 	*home;
	t_env *node;

	node = NULL;
	home = NULL;
	pwd = shell->env;
	if (!ft_strncmp(cmd, "cd", 3))
	{
		//avanza hasta que encuentra el pwd
		while (pwd && pwd->next && ft_strncmp((pwd->next)->name, "PWD", 3))
			pwd = pwd->next;
		if (!pwd || !pwd->next)
			return ;
		//busca el home
		home = shell->env;
		while (/*(u_cpy == p_cpy) ||*/ home && ft_strncmp(home->name, "HOME", 4))
			home = home->next;
		if (!home)
			return ;
		node = ft_create_node(ft_strdup("PWD"), ft_strdup(home->value));
		ft_add_node(&shell->env, pwd, node);
		return ;
	}
	if (*(cmd + 3) == '/')
	{
		pwd = shell->env;
		while (pwd && pwd->next && ft_strncmp((pwd->next)->name, "PWD", 3))
			pwd = pwd->next;
		if (!pwd)
			return ;
		if (access(cmd + 3, F_OK) == 0)
		{
			node = ft_create_node(ft_strdup("PWD"), ft_strdup(cmd + 3));
			ft_add_node(&shell->env, pwd, node);
			return ;
		}
		else
			return ;
	}
	if (!ft_strncmp(ft_strchr(cmd, ' ') + 1, "..", 2))
	{
		ft_errase_pwd(shell);
		temp = ft_strchr(cmd, '/');
		while (temp && !ft_strncmp(temp + 1, "..", 2))
		{
			ft_errase_pwd(shell);
			temp = ft_strchr(temp + 1, '/');
		}
	}
	else
		temp = ft_strchr(cmd, ' ');
	if (temp)
	{
		pwd = shell->env;
		while (pwd && pwd->next && ft_strncmp((pwd->next)->name, "PWD", 3))
			pwd = pwd->next;
		if (!pwd)
			return ;
		if (!ft_check_cd(temp + 1, (pwd->next)->value))
			return ;
		join = ft_strjoin((pwd->next)->value, "/");
		node = ft_create_node(ft_strdup("PWD"), ft_strjoin(join, temp + 1));
		ft_add_node(&shell->env, pwd, node);
	}
	//shell->env[p] = ft_strdup(shell->env[p]);
	//free(shell->env[p]);
//	shell->env[p] = ft_strjoin(temp, "\n");
//	free(temp);
}

void	ft_pwd(t_minishell *shell)
{
	t_env *env;

	env = shell->env;
	while (env && ft_strncmp(env->name, "PWD", 3))
		env = env->next;
	if (!env)
		ft_printf("te cagas\n");
	ft_printf("%s\n", ft_strchr(env->value, '/'));
	return ;
}

void	ft_print_env(t_env *env)
{
	if (!env)
		return ;
	while (env)
	{
		ft_printf("%s=%s\n", env->name, env->value);
		env = env->next;
	}
	return ;
}

void	ft_env(t_minishell *shell, char *cmd)
{
 	int	i;
	int	flag;
	t_env	*node;
	char	**var;
	t_env *env_tmp;
 
 	i = 0;
	flag = 0;
	var = ft_split(cmd, ' ');
	if (!var)
		return ;
	//Input es env unicamente
	if (!ft_strncmp(var[i], "env", ft_max_strlen(var[i], "env")) && !var[++i])
		return (ft_print_env(shell->env));
 	// Falta mirar si los caracteres son validos para nombre de variable
	env_tmp = ft_strdup_env(shell->env);
 	while (var[i])
 	{
 		if (ft_strchr(var[i], '='))
 		{
			if (!ft_check_duplicated(var[i], &env_tmp, NULL))
			{
				node = ft_create_node(ft_get_name(var[i]), ft_get_value(var[i]));
				ft_connect_node(&env_tmp, node);
			}
			flag = 1;
			
			//Dos opciones:
			//		1. La declaracion de la variable o cambio de valor va antes de env
			//			En este caso declaramos la variable en primera linea
			//		2. La declaracion de la variable o cambio de valor va despues de env
			//			En este caso declaramos la variable en ultima line
			//En ambos casos son variables temporales por lo que las metemos en env tmp
 		}
		if (!flag)
		{
			//hay que acabar el programa
			ft_printf("Wrong varibale declaration format\n");
			return ;
		}
		flag = 0;
 		i++;
 	}
	ft_print_env(env_tmp);
	return ;
 }

void	ft_print_export(t_env *export)
{
	t_env *tmp;

	tmp = export;
	while (tmp)
	{
		if (!tmp->value)
			ft_printf("declare -x %s\n", tmp->name);
		else if (tmp->value[0] == '\0')
			ft_printf("declare -x %s=\"\"\n", tmp->name);
		else
		{
			//Super chorra hay que cambairlo
			if (!(tmp->name[0] == '_' && tmp->name[1] == '\0'))
				ft_printf("declare -x %s=\"%s\"\n", tmp->name, tmp->value);
		}
		tmp = tmp->next;
	}
	return ;
}

void	ft_merge_lists(t_minishell **shell, t_env *first, t_env *second)
{
	t_env *export;
	t_env	*tmp;

	if ((*shell)->export)
		ft_free_env(&(*shell)->export);
	if (!second)
	{
		(*shell)->export = ft_strdup_env(first);
		return ;
	}
	export = ft_strdup_env(first);
	if (!export)
		return ;
	tmp = export;
	while (tmp->next)
		tmp = tmp->next;
	if (second)
		tmp->next = ft_strdup_env(second);
	(*shell)->export = export;
	return ;
}

void	ft_swap_list(t_env *a, t_env *b)
{
	char	*tmp_name = a->name;
	char	*tmp_value = a->value;

	a->name = b->name;
	a->value = b->value;

	b->name = tmp_name;
	b->value = tmp_value;
	return ;
}

size_t	ft_max_strlen(char *s1, char *s2)
{
	size_t	len1;
	size_t	len2;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);

	if (len1 > len2)
		return (len1);
	else
		return (len2);
}

void	ft_sort_list(t_env *head)
{
	t_env	*ptr;
	t_env	*lptr;
	int		swapped;

	if (!head)
		return ;
	swapped = 1;
	lptr = NULL;
	while (swapped)
	{
		swapped = 0;
		ptr = head;
		while (ptr->next != lptr)
		{
			if (ft_strncmp(ptr->name, ptr->next->name, ft_max_strlen(ptr->name ,ptr->next->name)) > 0)
			{
				ft_swap_list(ptr, ptr->next);
				swapped = 1;
			}
			ptr = ptr->next;
		}
		lptr = ptr;
	}
}

void	ft_export(t_minishell *shell, char *cmd)
{
	int	i;
	int flag;
	char **var;
	t_env *node;

	var = ft_split(cmd, ' ');
	if (!var)
		return ;
	i = 0;
	flag = 0;
	if (!ft_strncmp(var[i], "export", ft_max_strlen(var[i], "export")) && !var[++i])
		return (ft_sort_list(shell->export), ft_print_export(shell->export));
	while (var[i])
	{
		if (ft_strchr(var[i], '='))
		{
			if (!ft_check_duplicated(var[i], &shell->export, &shell->undefined_var))
			{
				node = ft_create_node(ft_get_name(var[i]), ft_get_value(var[i]));
				ft_connect_node(&shell->env, node);
				flag = 1;
			}
		}
		if (!flag)
		{	
			if (!ft_check_duplicated(var[i], &shell->env, &shell->undefined_var))
			{
				node = ft_create_node(ft_get_name(var[i]), ft_get_value(var[i]));
				ft_connect_node(&shell->undefined_var, node);
			}
		}
		flag = 0;
		i++;
	}
	ft_merge_lists(&shell, shell->env, shell->undefined_var);
	ft_sort_list(shell->export);
	ft_print_export(shell->export);
	return ;
}

void	ft_echo(char *cmd)
{
	char	*temp;

	temp = ft_strchr(cmd, ' ');
	if (ft_strncmp(temp + 1, "-n", 2) == 0)
		ft_printf("%s", temp + 4);
	else
		ft_printf("%s\n", temp + 1);
	return ;
}

void	ft_remove_var(t_minishell **shell, t_env *node, t_env **list)
{

	t_env *tmp;

	tmp = *list;
	while (tmp)
	{
		if (tmp == node)
		{
			ft_free_node(node, list);
			break ;
		}
		tmp = tmp->next;
	}
	ft_merge_lists(shell, (*shell)->env, (*shell)->undefined_var);
	ft_sort_list((*shell)->export);
	return ;
}

int	ft_check_in(t_minishell *shell, t_env **list, char *var)
{
	t_env *tmp;

	tmp = *list;
	while (tmp)
	{
		if (!ft_strncmp(var, tmp->name, ft_max_strlen(var, tmp->name)))
		{
			ft_remove_var(&shell, tmp, list);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

void	ft_unset(t_minishell *shell, char *cmd)
{
	int	i;
	char **vars;

	i = 0;
	vars = ft_split(cmd, ' ');
	while (vars && vars[i])
	{
		if (!ft_check_in(shell, &shell->env, vars[i]))
			ft_check_in(shell, &shell->undefined_var, vars[i]);
		i++;
	}
	return ;
}

void	ft_exec_build(t_minishell *shell, char *cmd)
{
	if (!ft_strncmp(cmd, "echo", ft_strlen("echo")))
		ft_echo(cmd);
	if (!ft_strncmp(cmd, "cd", ft_strlen("cd")))
	{
		ft_cd(shell, cmd);
		ft_pwd(shell);
	}
	if (ft_strncmp(cmd, "pwd", 3) == 0)
		ft_pwd(shell);
	if (!ft_strncmp(cmd, "export", ft_strlen("export")))
		ft_export(shell, cmd);
	if (!ft_strncmp(cmd, "unset", ft_strlen("unset")))
		ft_unset(shell, cmd + 6);
	if (!ft_strncmp(cmd, "env", ft_strlen("env")))
		ft_env(shell, cmd);
	if (!ft_strncmp(cmd, "exit", ft_strlen("exit")))
	{
		ft_free_minishell(&shell);
		exit(0) ;
	}
	return ;
	//ft_free_minishell(&shell);
	//exit(0);
}

void	ft_pre_exec_command(t_pipex *pipex, t_token *cmd, t_minishell *shell)
{
	char	*temp;
	char	*path_command;
	char	**command;
	char **env;

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
	env = ft_create_array_env(shell->env);
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
		ft_pre_exec_command(pipex, save, shell);
	}
}

char	*ft_find_path(t_env **env)
{
	t_env *tmp;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->name, "PATH", 4))
			return (tmp->value);
		tmp = tmp->next;
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

int	ft_path(t_env **env, t_pipex **pipex, char *cmd)
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
	(*pipex)->childs = 0;
	(*pipex)->docs_in = NULL;
	(*pipex)->docs_out = NULL;
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
	ft_init_pipex(&pipex);
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
					ft_manage_child_signals();
					pipex->command = ft_split(save->str, ' ');
					if (ft_path(&shell->env, &pipex, pipex->command[0]) == 0)
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
				ft_manage_child_signals();
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
//Me falta el unset, el env -i y hay una variable que en el env sale y en el export no, en el original, una que es como _no se que