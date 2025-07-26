/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 22:08:46 by lbellmas          #+#    #+#             */
/*   Updated: 2025/07/25 22:10:16 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

char	*ft_find_path(t_env **env)
{
	t_env	*tmp;

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
	return (1);
}

int	ft_path2(t_pipex **pipex, char **split, char *cmd)
{
	char	*path;
	int		p;
	int		res;
	char	*temp;

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
			return (res);
		}
		ft_safe_free((void **)&path);
		p++;
	}
	write (2, "command not found\n", 18);
	return (ft_free_array(split), 0);
}

int	ft_path(t_env **env, t_pipex **pipex, char *cmd)
{
	char	**split;
	char	*temp;

	if (!env || !*env)
		return (0);
	if ((*pipex)->path)
		ft_safe_free((void **)&(*pipex)->path);
	temp = ft_find_path(env);
	if (!temp)
		return (0);
	split = ft_split(temp, ':');
	if (!split)
		return (0);
	return (ft_path2(pipex, split, cmd));
}
