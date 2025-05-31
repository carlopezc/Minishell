/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 18:09:11 by lbellmas          #+#    #+#             */
/*   Updated: 2025/05/31 19:15:21 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"
#include <fcntl.h>
#include <sys/wait.h>

void	ft_add_node(t_env **list, t_env *prev, t_env *node)
{
	t_env	*tmp;
	t_env	*next;

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
	while (old_pwd && old_pwd->next
		&& ft_strncmp((old_pwd->next)->name, "OLDPWD",
			ft_max_strlen("OLDPWD", (pwd->next)->name)))
		old_pwd = old_pwd->next;
	new = ft_create_node(ft_strdup("OLDPWD"), ft_strdup(pwd->value));
	ft_add_node(env, old_pwd, new);
}

char	*ft_correct_cd(char *path)
{
	char	*dots;
	char	*first;
	int		p;
	int		i;

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
		(first[i++]) = (dots[p++]);
	first[i] = '\0';
	return (path);
}

void	ft_cd_home(t_minishell *shell)
{
	t_env	*node;
	t_env	*pwd;
	t_env	*home;

	home = NULL;
	node = NULL;
	pwd = shell->env;
	while (pwd && pwd->next && ft_strncmp((pwd->next)->name,
			"PWD", ft_max_strlen("PWD", (pwd->next)->name)))
		pwd = pwd->next;
	if (!pwd || !pwd->next)
		return ;
	home = shell->env;
	while (home && ft_strncmp(home->name, "HOME",
			ft_max_strlen("HOME", home->name)))
		home = home->next;
	if (!home)
		return ;
	ft_old_pwd(&shell->env, pwd->next);
	node = ft_create_node(ft_strdup("PWD"), ft_strdup(home->value));
	ft_add_node(&shell->env, pwd, node);
	chdir(home->value);
	return ;
}

void	ft_cd_add(t_minishell *shell, char *temp)
{
	t_env	*pwd;
	t_env	*node;
	char	*join;

	pwd = shell->env;
	while (pwd && pwd->next && ft_strncmp((pwd->next)->name,
			"PWD", ft_max_strlen("PWD", (pwd->next)->name)))
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
