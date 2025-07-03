/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:16:23 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/01 15:21:30 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

char	**ft_create_array_env(t_env *env)
{
	int		i;
	char	**env_array;
	char	*tmp;

	if (!env)
		return (NULL);
	i = ft_listlen(env);
	env_array = (char **)malloc((i + 1) * sizeof(char *));
	if (!env_array)
		return (NULL);
	env_array[i] = NULL;
	i = 0;
	while (env)
	{
		if (env->name && env->value)
		{
			tmp = ft_strjoin(env->name, "=");
			env_array[i++] = ft_strjoin(tmp, env->value);
			ft_safe_free((void **)&tmp);
		}
		else
			env_array[i++] = env->name;
		env = env->next;
	}
	return (env_array);
}

static char	*ft_create_env2(char *env)
{
	char	*tmp;
	char	*value;
	int	num;

	tmp = ft_get_value(env);
	num = ft_atoi(tmp) + 1;
	if (num > 999)
		num = 1;
	value = ft_itoa(num);
	ft_safe_free((void **)&tmp);
	return (value);
}

t_env	*ft_create_env(char **env_array)
{
	t_env	*env;
	t_env	*node;
	char	*name;
	char	*value;
	int		i;

	if (!env_array || !*env_array)
		return (NULL);
	i = -1;
	env = NULL;
	while (env_array[++i])
	{
		name = ft_get_name(env_array[i]);
		if (!ft_strncmp(name, "SHLVL", ft_max_strlen(name, "SHLVL")))
			value = ft_create_env2(env_array[i]);
		else
			value = ft_get_value(env_array[i]);
		node = ft_create_node(ft_strdup(name), ft_strdup(value));
		ft_safe_free((void **)&value);
		ft_safe_free((void **)&name);
		if (!node)
			return (ft_printf("Error creating environment node\n"), NULL);
		ft_connect_node(&env, node);
	}
	return (env);
}

t_env	*ft_empty_env(void)
{
	t_env	*env;
	t_env	*node;
	char	*cwd;

	env = NULL;
	node = ft_create_node(ft_strdup("SHLVL"), ft_strdup("1"));
	if (!node)
		return (NULL);
	ft_connect_node(&env, node);
	cwd = getcwd(NULL, 0);
	node = ft_create_node(ft_strdup("PWD"), ft_strdup(cwd));
	if (!node)
		return (ft_safe_free((void **)&cwd), NULL);
	ft_safe_free((void **)&cwd);
	ft_connect_node(&env, node);
	node = ft_create_node(ft_strdup("_"), ft_strdup("./minishell"));
	if (!node)
		return (NULL);
	ft_connect_node(&env, node);
	return (env);
}
