/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:52:48 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/30 21:52:13 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_free_tokens(t_minishell **minishell)
{
	t_token	*tmp;
	t_token	*token;

	token = (*minishell)->tokens;
	tmp = NULL;
	while (token)
	{
		tmp = token->next;
		if (token->str)
			ft_safe_free((void **)&(token->str));
		ft_safe_free((void **)&token);
		token = tmp;
	}
	return ;
}

static int	ft_if_no_next(t_env **tmp, t_env **list)
{
	if (*tmp && !(*tmp)->next)
	{
		ft_safe_free((void **)&(*tmp)->name);
		ft_safe_free((void **)&(*tmp)->value);
		ft_safe_free((void **)tmp);
		*list = NULL;
		list = NULL;
		return (0);
	}
	return (1);
}

void	ft_free_node(t_env *node, t_env **list)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = *list;
	prev = NULL;
	if (!node)
		return ;
	if (!ft_if_no_next(&tmp, list))
		return ;
	while (tmp)
	{
		if (tmp->next && !ft_strncmp((tmp->next)->name,
				node->name, ft_max_strlen((tmp->next)->name, node->name)))
			prev = tmp;
		if (!ft_strncmp(tmp->name, node->name,
				ft_max_strlen(tmp->name, node->name)))
		{
			if (prev)
				prev->next = tmp->next;
			else
				*list = tmp->next;
			ft_safe_free((void **)&tmp->value);
			tmp->next = NULL;
			return (ft_safe_free((void **)&tmp->name),
				ft_safe_free((void **)&tmp));
		}
		tmp = tmp->next;
	}
	return ;
}

void	ft_free_minishell(t_minishell **minishell)
{
	if (!minishell || !*minishell)
		return ;
	if ((*minishell)->tokens)
	{
		ft_free_tokens(minishell);
		(*minishell)->tokens = NULL;
	}
	if ((*minishell)->env)
		ft_free_env(&(*minishell)->env);
	if ((*minishell)->export)
		ft_free_env(&(*minishell)->export);
	if ((*minishell)->undefined_var)
		ft_free_env(&(*minishell)->undefined_var);
	free(*minishell);
	*minishell = NULL;
	return ;
}
