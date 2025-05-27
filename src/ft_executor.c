/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:37:21 by lbellmas          #+#    #+#             */
/*   Updated: 2025/05/27 16:21:51 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../get_next_line/get_next_line_bonus.h"
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

void	ft_old_pwd(t_env **env, t_env *pwd)
{
	t_env	*old_pwd;
	t_env	*new;

	old_pwd = *env;
	while (old_pwd && old_pwd->next && ft_strncmp((old_pwd->next)->name, "OLDPWD", ft_max_strlen("OLDPWD", (pwd->next)->name)))
		old_pwd = old_pwd->next;
	new = ft_create_node(ft_strdup("OLDPWD"), ft_strdup(pwd->value));
	ft_add_node(env, old_pwd, new);
}

char	*ft_correct_cd(char *path)
{
	char	*dots;
	char	*first;
	int	p;
	int i;

	p = 4;
	i = 1;
	dots = ft_strnstr(path, "/../", ft_strlen(path));
	while (*(dots - i) != '/' && (dots - i != path))
		i++;
	if (dots - i == path)
		first = ft_strchr(path, ' ') + 1;
	else
		first = dots - i + 1;
	(*first) = '\0';
	if (!(dots + p))
		return (path);
	i = 0;
	while (dots[p])
	{
		(first[i]) = (dots[p]);
		p++;
		i++;
	}
	first[i] = '\0';
	return (path);
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
	while (pwd && pwd->next && ft_strncmp((pwd->next)->name, "PWD", ft_max_strlen("PWD", (pwd->next)->name)))
		pwd = pwd->next;
	if (!ft_strncmp(cmd, "cd", 3) || !(ft_strncmp(cmd, "cd ~", 5)))
	{
		//avanza hasta que encuentra el pwd
		while (pwd && pwd->next && ft_strncmp((pwd->next)->name, "PWD", ft_max_strlen("PWD", (pwd->next)->name)))
			pwd = pwd->next;
		if (!pwd || !pwd->next)
			return ;
		//busca el home
		home = shell->env;
		while (/*(u_cpy == p_cpy) ||*/ home && ft_strncmp(home->name, "HOME", ft_max_strlen("HOME", home->name)))
			home = home->next;
		if (!home)
			return ;
		ft_old_pwd(&shell->env, pwd->next);
		node = ft_create_node(ft_strdup("PWD"), ft_strdup(home->value));
		ft_add_node(&shell->env, pwd, node);
		chdir(home->value);
		return ;
	}
	if (*(cmd + 3) == '/')
	{
		pwd = shell->env;
		while (pwd && pwd->next && ft_strncmp((pwd->next)->name, "PWD", ft_max_strlen("PWD", (pwd->next)->name)))
			pwd = pwd->next;
		if (!pwd)
			return ;
		if (access(cmd + 3, F_OK) == 0)
		{
			ft_old_pwd(&shell->env, pwd->next);
			node = ft_create_node(ft_strdup("PWD"), ft_strdup(cmd + 3));
			ft_add_node(&shell->env, pwd, node);
			chdir(cmd + 3);
			return ;
		}
		else
			return ;
	}
	ft_old_pwd(&shell->env, pwd ->next);
	if (!ft_strncmp(ft_strchr(cmd, ' ') + 1, "..", 2))
	{
		pwd = shell->env;
		while (pwd && pwd->next && ft_strncmp((pwd->next)->name, "PWD", ft_max_strlen("PWD", (pwd->next)->name)))
			pwd = pwd->next;
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
		while (pwd && pwd->next && ft_strncmp((pwd->next)->name, "PWD", ft_max_strlen("PWD", (pwd->next)->name)))
			pwd = pwd->next;
		if (!pwd)
			return ;
		if (!ft_check_cd(temp + 1, (pwd->next)->value))
			return ;
		join = ft_strjoin((pwd->next)->value, "/");
		temp = ft_strjoin(join, temp + 1);
		if (temp[ft_strlen(temp) - 1] == '/')
			temp[ft_strlen(temp) - 1] = '\0';
		while (ft_strnstr(temp, "../", ft_strlen(temp)))
			temp = ft_correct_cd(temp);
		node = ft_create_node(ft_strdup("PWD"), temp);
		ft_add_node(&shell->env, pwd, node);
	}
	if (!pwd->next->value || pwd->next->value[0] == '\0')
		pwd->next->value = ft_strdup("/");
	chdir(pwd->next->value);
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
	//En BASH al declarar variables temporales (con env) puedo nombrar las variables como quiera
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

int	ft_check_name(char *var)
{
	int	i;

	i = 0;
	if (!ft_isalpha(var[i]) && (var[i] != '_'))
		return (ft_printf("Non valid name\n"), 0);
	while (var[++i] && var[i] != '=')
	{
		if (!ft_isalnum(var[i]) && (var[i] != '_'))
			return (ft_printf("Non valid name\n"), 0);
	}
	return (1);
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
 	// NOMBRES VÁLIDOS:
	// Tiene que comenzar con caracter o con guion bajo
	// En el resto del nombre puede contener caracteres guiones bajos y números NADA MAS
	// Para el valor no hay restricciones, cualquier simbolo, emoji, caracter etc, cuidado con caracteres especiales escaparlos, o entre comillas simples
	if (!ft_strncmp(var[i], "export", ft_max_strlen(var[i], "export")) && !var[++i])
		return (ft_sort_list(shell->export), ft_print_export(shell->export));
	while (var[i])
	{
		if (!ft_check_name(var[i]))
			return ;
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
		ft_merge_lists(&shell, shell->env, shell->undefined_var);
		ft_sort_list(shell->export);
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
//	ft_printf("Pre exec command: \n");
//	ft_printf("Ruta buena : %s\n", getcwd(NULL, 0));
//	ft_print_array(env);
	execve(path_command, command, env);
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

static void	ft_docs_in_child(t_pipex *pipex)
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
	while (pipex->docs_in[p])
	{
		ft_join_docs(pipex->docs_in[p]);
		p++;
	}
	exit(0);
}

void	ft_docs_in(t_pipex *pipex)
{
	int		tmp_pipe[2];
	pipe(tmp_pipe);
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
		close(pipex->pipe[0][0]);
		close(pipex->pipe[0][1]);
		waitpid(pipex->pid, NULL, 0);
		close(tmp_pipe[1]);
		dup2(tmp_pipe[0], 0);
		close(tmp_pipe[0]);
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
	{
		if (pipex->docs_out[1] == 0 && pipex->pipe[1][1] == 0)
		{
			dup2(pipex->docs_out[0], 1);
			close(pipex->docs_out[0]);
		}
		else
		{
			char	*str;
			char	*trash = ft_strdup("");
			int	tmp_pipe[2];
			int	p = 0;
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
				str = get_next_line(tmp_pipe[0]);
				while (str)
				{
					trash = ft_strjoin(trash, str);
					free(str);
					str = get_next_line(tmp_pipe[0]);
				}
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
		}
	}
	else if (pipex->pipe[1][1])
	{
		close(pipex->pipe[1][0]);
		dup2(pipex->pipe[1][1], 1);
		close(pipex->pipe[1][1]);
	}
	if (save->type == BUILTIN)
	{
		ft_exec_build(shell, save->str);
		if (pipex->childs != 0)
			exit (0);
	}
	else
		ft_pre_exec_command(pipex, save, shell);
}

char	*ft_find_path(t_env **env)
{
	t_env *tmp;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->name, "PATH", ft_max_strlen(tmp->name, "PATH")))
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
}

static t_token	*ft_redir2(t_pipex *pipex, t_token *save, t_token_type type)
{
	int	count;

	count = 0;
	while (save && save->type == type)
	{
		if (type == REDIR_IN)
			pipex->docs_in[count] = open(save->str, O_RDONLY);
		else if (type == APPEND)
			pipex->docs_out[count] = open(save->str, O_WRONLY | O_APPEND | O_CREAT, 0777);
		else
			pipex->docs_out[count] = open(save->str, O_WRONLY | O_CREAT, 0777);
		count++;
		save = save->next;
	}
	return (save);
}

t_token *ft_redir(t_token *save, t_token_type type, t_pipex *pipex)
{
	t_token *temp;
	int	count;


	temp = save;
	count = 1;
	while (temp && temp->type == type)
	{
		count++;
		temp = temp->next;
	}
	if (type == REDIR_IN)
		pipex->docs_in = (int *)malloc(count * sizeof(int));
	else
		pipex->docs_out = (int *)malloc(count * sizeof(int));
	if (!pipex->docs_in && !pipex->docs_out)
		exit(0);
	save = ft_redir2(pipex, save, type);
	if (type == REDIR_IN)
		pipex->docs_in[count - 1] = 0;
	else
		pipex->docs_out[count - 1] = 0;
	return (save);
}

t_pipex	*ft_init_pipex()
{
	t_pipex *pipex;

	pipex = (t_pipex *)malloc(sizeof(t_pipex));
	pipex->childs = 0;
	pipex->docs_in = NULL;
	pipex->docs_out = NULL;
	pipex->command = NULL;
	pipex->path = NULL;
	pipex->pid = 0;
	pipex->brackets_count = 0;
	pipex->heredoc = 0;
	pipex->pipe[0][0] = 0;
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

void	ft_terminator(t_pipex *pipex)
{
	while (pipex->childs > 0)
	{
		wait(NULL);
		pipex->childs--;
	}
}

t_token	*ft_analisis_comands(t_pipex *pipex, t_minishell *shell, t_token **save)
{
	t_token	*tmp;

	if (ft_strncmp("exit", (*save)->str, 5) == 0)
		exit(0);
	tmp = *save;
	if ((*save)->type == COMMAND || (*save)->type == EXEC || !ft_strncmp("pwd", (*save)->str, 3) || !ft_strncmp("echo", (*save)->str, 4) || (ft_strncmp("cd", (*save)->str, 2) || ((!ft_strncmp("env", (*save)->str, 3) && ft_strncmp("env ", (*save)->str, 4)) || (!ft_strncmp("export", (*save)->str, 6) && ft_strncmp("export ", (*save)->str, 7)))))
	{
		while (*save && (*save)->type != PIPE && (*save)->type != AND && (*save)->type != OR)
			*save = (*save)->next;
		if (*save && (*save)->type == PIPE)
			pipe(pipex->pipe[1]);
		pipex->pid = fork();
		pipex->childs++;
		if (pipex->pid == 0 && tmp->type == COMMAND)
		{
			ft_manage_child_signals();
			pipex->command = ft_split(tmp->str, ' ');
			if (ft_path(&shell->env, &pipex, pipex->command[0]) == 0)
				exit (0);
		}
		*save = tmp->next;
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
			return (save) ;
	}
	else
		save = save->next;
	return (save);
}

t_token	*ft_exectime(t_pipex *pipex, t_minishell *shell, t_token *tmp, t_token *save)
{
	if (pipex->pid == 0 && tmp)
	{
		ft_manage_child_signals();
		ft_exec(shell, pipex, tmp);
		save = save->next;
	}
	else
	{
		while (save && save->type != PIPE && save->type != AND && save->type != OR)
		{
			if (save->type == C_BRACKET)
				pipex->brackets_count--;
			save = save->next;
		}
	}
	return (save);
}

t_token	*ft_and(t_pipex *pipex, t_minishell *shell, t_token *save)
{
	int	brackets;

	brackets = pipex->brackets_count;
	while (pipex->childs > 0)
	{
		wait(&shell->status);
		pipex->childs--;
	}
	if (shell->status != 0)
	{
		while (save && (save->type != AND || pipex->brackets_count != brackets) && (save->type != OR || brackets != pipex->brackets_count))
		{
			if (save->type == O_BRACKET)
				pipex->brackets_count += 1;
			if (save->type == C_BRACKET)
				pipex->brackets_count -= 1;
			save = save->next;
		}
	}
	return (save);
}

t_token	*ft_or(t_pipex *pipex, t_minishell *shell, t_token *save)
{
	int	brackets;

	brackets = pipex->brackets_count;
	while (pipex->childs > 0)
	{
		wait(&shell->status);
		pipex->childs--;
	}
	if (shell->status == 0)
	{
		while (save && (save->type != AND || pipex->brackets_count != brackets) && (save->type != OR || brackets != pipex->brackets_count))
		{
			if (save->type == O_BRACKET)
				pipex->brackets_count += 1;
			if (save->type == C_BRACKET)
				pipex->brackets_count -= 1;
			save = save->next;
		}
	}
	return (save);
}

t_token	*ft_killchilds(t_pipex *pipex, t_minishell *shell, t_token *save)
{
	if (save && save->type == AND)
		save = ft_and(pipex, shell, save->next);
	else if (save && save->type == OR)
		save = ft_or(pipex, shell, save->next);
	else
	{
		while (pipex->childs > 0)
		{
			wait(&shell->status);
			pipex->childs--;
		}
	}
	if (save)
		return (save);
	return (ft_terminator(pipex), NULL);
}

int	ft_executor(t_minishell *shell)
{
	t_token	*save;
	t_token	*tmp = NULL;
	t_pipex	*pipex;

	pipex = ft_init_pipex();
	save = shell->tokens;
	while (save)
	{
		while (save && save->type != AND && save->type != OR)
		{
			if (save && save->type == O_BRACKET)
			{
				pipex->brackets_count += 1;
				save = save->next;
			}
			while (save && save->type != PIPE && save->type != AND && save->type != OR)//(save && save->type != AND && save->type != OR)
			{
				if (save->type == COMMAND || save->type == BUILTIN || save->type == EXEC)
					tmp = ft_analisis_comands(pipex, shell, &save);
				while (pipex->pid == 0 && save && (save->type == REDIR_IN || save->type == REDIR_OUT || save->type == HEREDOC || save->type == APPEND))
					save = ft_analisis_redir(save, pipex);
				save = ft_exectime(pipex, shell, tmp, save);
				if (save && save->type == C_BRACKET)
				{
					pipex->brackets_count -= 1;
					save = save->next;
				}
			}
			if (save && save->type == PIPE)
				save = save->next;
			ft_arrange_fd(pipex);
		}
		save = ft_killchilds(pipex, shell, save);
	}
	return (ft_free_pipex(pipex), 0);
}
