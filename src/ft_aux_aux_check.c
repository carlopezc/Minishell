/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_aux_aux_check.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 00:41:58 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/26 02:56:08 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

static int	ft_check_duplicated2(t_env **tmp, char *name_to_add,
	char *value, char *str)
{
	while (*tmp)
	{
		if (!ft_strncmp(name_to_add,
				(*tmp)->name, ft_max_strlen(name_to_add, (*tmp)->name)))
		{
			if (value)
				ft_change_value(str, tmp);
			ft_safe_free((void **)&name_to_add);
			ft_safe_free((void **)&value);
			return (0);
		}
		*tmp = (*tmp)->next;
	}
	return (1);
}

int	ft_check_undefined(char *name, t_env **undefined)
{
	t_env	*tmp;

	tmp = *undefined;
	while (tmp)
	{
		if (!ft_strncmp(name, tmp->name, ft_max_strlen(name, tmp->name)))
		{
			ft_free_node(tmp, undefined);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	ft_check_duplicated(char *str, t_env **env, t_env **undefined)
{
	t_env	*tmp;
	char	*name;
	char	*value;

	tmp = *env;
	name = ft_get_name(str);
	value = ft_get_value(str);
	if (!ft_check_duplicated2(&tmp, name, value, str))
		return (1);
	if (undefined && ft_check_undefined(name, undefined))
	{
		ft_safe_free((void **)&name);
		return (0);
	}
	ft_safe_free((void **)&value);
	ft_safe_free((void **)&name);
	return (0);
}

void	ft_check_next2(char *str, int *i, int *c_brckt)
{
	while ((str)[*i])
	{
		if ((str)[*i] && (str)[*i] == ')' && ((!(*i)
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
		i = 0;
		tmp = tmp->next;
	}
	return (1);
}
