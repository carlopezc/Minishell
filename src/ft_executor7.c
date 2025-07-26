/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor7.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 18:14:50 by lbellmas          #+#    #+#             */
/*   Updated: 2025/07/26 01:41:46 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"
#include <fcntl.h>
#include <sys/wait.h>

void	ft_remove_var(t_minishell **shell, t_env *node, t_env **list)
{
	t_env	*tmp;

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
	t_env	*tmp;

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
	int		i;
	char	**vars;

	i = 0;
	vars = ft_split(cmd, ' ');
	while (vars && vars[i])
	{
		if (!ft_check_in(shell, &shell->env, vars[i]))
			ft_check_in(shell, &shell->undefined_var, vars[i]);
		i++;
	}
	if (vars)
		ft_free_array(vars);
	return ;
}

void	ft_check_pwd(t_minishell *shell, char *cmd)
{
	if (ft_strncmp(cmd, "pwd ", 4) == 0)
	{
		write(2, "pwd: too many arguments\n", 24);
		exit (1);
	}
	if (ft_strncmp(cmd, "pwd", 3) == 0)
		ft_pwd(shell);
}

void	ft_exec_build(t_minishell *shell, char *cmd)
{
	if (ft_strlen(cmd) == 4 && !ft_strncmp(cmd, "echo", ft_strlen("echo")))
	{
		write(1, "\n", 1);
		return ;
	}
	if (!ft_strncmp(cmd, "echo", ft_strlen("echo")))
		ft_echo(cmd);
	if (!ft_strncmp(cmd, "cd", ft_strlen("cd")))
	{
		ft_cd(shell, cmd);
		ft_merge_lists(&shell, shell->env, shell->undefined_var);
		ft_sort_list(shell->export);
	}
	ft_check_pwd(shell, cmd);
	if (!ft_strncmp(cmd, "export", ft_strlen("export")))
		ft_export(shell, cmd);
	if (!ft_strncmp(cmd, "unset", ft_strlen("unset")))
		ft_unset(shell, cmd + 6);
	if (!ft_strncmp(cmd, "env", ft_strlen("env")))
		ft_env(shell, cmd);
}
