/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:16:23 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/29 17:50:48 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minsihell.h"

t_env	*ft_create_node(char *name, char *value)
{
	t_env	*node;

	if (!name)
		return (NULL);
	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->name = name;
	if (!value)
		node->value = NULL;
	else
		node->value = value;
	node->next = NULL;
	return (node);
}

void	ft_connect_node(t_env **env, t_env *node)
{
	t_env	*tmp;

	if (!*env)
	{
		*env = node;
		return ;
	}
	tmp = *env;
	while (tmp && tmp->next)
		tmp = tmp->next;
	tmp->next = node;
	return ;
}

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

t_env	*ft_create_env(char **env_array)
{
	t_env	*env;
	t_env	*node;
	char	*name;
	char	*value;
	int		i;

	if (!env_array || !*env_array)
		return (NULL);
	i = 0;
	env = NULL;
	while (env_array[i])
	{
		name = ft_get_name(env_array[i]);
		if (!ft_strncmp(name, "SHLVL", ft_max_strlen(name, "SHLVL")))
			value = ft_itoa(ft_atoi(ft_get_value(env_array[i])) + 1);
		else
			value = ft_get_value(env_array[i]);
		node = ft_create_node(ft_strdup(name), ft_strdup(value));
		if (!node)
			return (ft_printf("Error creating environment node\n"), NULL);
		ft_connect_node(&env, node);
		i++;
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
