/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor5.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 18:12:04 by lbellmas          #+#    #+#             */
/*   Updated: 2025/06/01 18:51:48 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"
#include <fcntl.h>
#include <sys/wait.h>

static int	ft_env2(char **var, int i, t_env *env_tmp)
{
	int		flag;
	t_env	*node;

	flag = 0;
	if (ft_strchr(var[i], '='))
	{
		if (!ft_check_duplicated(var[i], &env_tmp, NULL))
		{
			node = ft_create_node(ft_get_name(var[i]), ft_get_value(var[i]));
			ft_connect_node(&env_tmp, node);
		}
		flag = 1;
	}
	if (!flag)
	{
		ft_printf("Wrong varibale declaration format\n");
		return (-1);
	}
	i++;
	return (i);
}

void	ft_env(t_minishell *shell, char *cmd)
{
	int		i;
	char	**var;
	t_env	*env_tmp;

	i = 0;
	var = ft_split_cmd(cmd, ' ');
	if (!var)
		return ;
	if (!ft_strncmp(var[i], "env", ft_max_strlen(var[i], "env")) && !var[++i])
		return (ft_print_env(shell->env));
	env_tmp = ft_strdup_env(shell->env);
	while (i != -1 && var[i])
		i = ft_env2(var, i, env_tmp);
	ft_print_env(env_tmp);
	return ;
}

void	ft_merge_lists(t_minishell **shell, t_env *first, t_env *second)
{
	t_env	*export;
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
	char	*tmp_name;
	char	*tmp_value;

	tmp_name = a->name;
	tmp_value = a->name;
	a->name = b->name;
	a->value = b->value;
	b->name = tmp_name;
	b->value = tmp_value;
	return ;
}
