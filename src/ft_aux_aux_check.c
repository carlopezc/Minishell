/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_aux_aux_check.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 00:41:58 by carlopez          #+#    #+#             */
/*   Updated: 2025/06/01 00:43:18 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

static int	ft_check_duplicated2(t_env *tmp, char *name_to_add,
		char *value, char *str)
{
	while (tmp)
	{
		if (!ft_strncmp(name_to_add,
				tmp->name, ft_max_strlen(name_to_add, tmp->name)))
		{
			if (value)
				ft_change_value(str, &tmp);
			ft_safe_free((void **)&name_to_add);
			return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

int	ft_check_duplicated(char *str, t_env **env, t_env **undefined)
{
	char	*name_to_add;
	char	*value;
	t_env	*tmp;

	tmp = *env;
	name_to_add = ft_get_name(str);
	value = ft_get_value(str);
	if (!ft_check_duplicated2(tmp, name_to_add, value, str))
		return (1);
	if (undefined)
	{
		tmp = *undefined;
		while (tmp)
		{
			if (!ft_strncmp(name_to_add, tmp->name,
					ft_max_strlen(name_to_add, tmp->name)))
			{
				ft_free_node(tmp, undefined);
				ft_safe_free((void **)&name_to_add);
				return (0);
			}
			tmp = tmp->next;
		}
	}
	return (ft_safe_free((void **)&name_to_add), 0);
}

void	ft_check_next2(char *str, int *i, int *c_brckt)
{
	while ((str)[*i])
	{
		if ((str)[*i] && (str)[*i] == ')' && ((!(str)[*i - 1]
				|| (str)[*i - 1] != '\\')))
			(*c_brckt)++;
		(*i)++;
	}
}

int	ft_check_next(t_token *tokens, int o_brckt)
{
	t_token	*tmp;
	int		c_brckt;
	int		i;

	tmp = tokens;
	c_brckt = 0;
	i = 0;
	while (tmp)
	{
		ft_check_next2(tmp->str, &i, &c_brckt);
		if (c_brckt == o_brckt)
		{
			if (!ft_quit_brckt_dup(tmp, ')'))
				return (0);
			return (2);
		}
		c_brckt = 0;
		i = -1;
		tmp = tmp->next;
	}
	return (1);
}
