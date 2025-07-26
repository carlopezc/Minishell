/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 18:10:29 by lbellmas          #+#    #+#             */
/*   Updated: 2025/07/26 05:51:08 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_check_erraser(t_env *pwd, t_minishell *shell)
{
	if (access(pwd->next->value, F_OK) != 0)
	{
		ft_cd_home(shell);
		write (2, "No such file or directory\n", 26);
		return (ft_finish_build(1, shell), 0);
	}
	return (1);
}

char	*ft_cd_erraser(t_minishell *shell, char *cmd)
{
	t_env	*pwd;
	char	*temp;

	if (!ft_strncmp(ft_strchr(cmd, ' ') + 1, "..", 2))
	{
		pwd = shell->env;
		while (pwd && pwd->next && ft_strncmp((pwd->next)->name,
				"PWD", ft_max_strlen("PWD", (pwd->next)->name)))
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
		return (ft_strchr(cmd, ' '));
	if (ft_check_erraser(pwd, shell) == 0)
		return (NULL);
	return (temp);
}

void	ft_cd(t_minishell *shell, char *cmd)
{
	char	*temp;
	t_env	*pwd;

	pwd = shell->env;
	while (pwd && pwd->next && ft_strncmp((pwd->next)->name,
			"PWD", ft_max_strlen("PWD", (pwd->next)->name)))
		pwd = pwd->next;
	if ((!pwd || !pwd->next) || !ft_strncmp(cmd, "cd", 3)
		|| !(ft_strncmp(cmd, "cd ~", 5)))
		return (ft_cd_home(shell));
	if (!(ft_strncmp(cmd, "cd ~", 4)))
	{
		ft_cd_home(shell);
		if (ft_strlen(cmd) > 4)
			cmd = ft_strjoin("cd ", cmd + 5);
	}
	if (*(cmd + 3) == '/')
		return (ft_cd_route(shell, cmd));
	ft_old_pwd(&shell->env, pwd->next);
	temp = ft_cd_erraser(shell, cmd);
	if (temp)
		ft_cd_add(shell, temp);
	if (!pwd->next->value || pwd->next->value[0] == '\0')
		pwd->next->value = ft_strdup("/");
	chdir(pwd->next->value);
}

void	ft_pwd(t_minishell *shell)
{
	t_env	*env;

	env = shell->env;
	while (env && ft_strncmp(env->name, "PWD", 3))
		env = env->next;
	if (!env)
		ft_printf("no env\n");
	if (ft_strchr(env->value, '/'))
	{
		ft_printf("%s\n", ft_strchr(env->value, '/'));
		return ;
	}
	env = shell->env;
	while (env && ft_strncmp(env->name, "OLDPWD", 3))
		env = env->next;
	if (!env)
		ft_printf("no env\n");
	ft_printf("%s\n", ft_strchr(env->value, '/'));
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
