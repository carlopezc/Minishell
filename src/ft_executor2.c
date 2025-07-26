/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 18:08:11 by lbellmas          #+#    #+#             */
/*   Updated: 2025/07/26 16:41:39 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_safe_free(void **p)
{
	if (p && *p)
	{
		free(*p);
		*p = NULL;
	}
	return ;
}

void	ft_close_all(int *files)
{
	int p;

	p = 0;
	while (files[p])
	{
		close(files[p]);
		p++;
	}
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
		ft_close_all(pipex->docs_in);
	ft_safe_free((void **)&pipex->docs_in);
	if (pipex->docs_out)
		ft_close_all(pipex->docs_out);
	ft_safe_free((void **)&pipex->docs_out);
	if (pipex->heredoc)
		close(pipex->heredoc);
}

int	ft_check_cd(char *file, char *pwd, t_minishell *shell)
{
	char	*temp;
	char	*final;

	temp = ft_strjoin(pwd, "/");
	final = ft_strjoin(temp, file);
	ft_safe_free((void **)&temp);
	if (access(final, F_OK) != 0 || access(final, X_OK) != 0)
		return (write (2, "Cannot be accessed\n", 19),
			ft_safe_free((void **)&final), ft_finish_build(1, shell), 0);
	else
		return (ft_safe_free((void **)&final), 1);
}

void	ft_errase_pwd(t_minishell *shell)
{
	int		c;
	t_env	*env;

	env = shell->env;
	while (env && ft_strncmp(env->name, "PWD", 3))
		env = env->next;
	if (!env || ft_strncmp("/", env->value, 2) == 0)
		return ;
	c = ft_strlen(env->value) - 1;
	while (env->value[c] != '/')
	{
		if (c == 0)
			return ;
		env->value[c--] = '\0';
	}
	if (c != 0)
		env->value[c] = '\0';
}

char	*ft_errase_route(char *route)
{
	int	c;

	c = ft_strlen(route) - 1;
	while (route[c] != '/')
	{
		if (c == 0)
			return (route);
		route[c--] = '\0';
	}
	if (route[c] != '=' && route[c -1] != '=')
		route[c] = '\0';
	return (route);
}
