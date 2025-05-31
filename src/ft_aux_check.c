/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_aux_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 18:31:30 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/30 21:31:34 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_check_duplicated(char *str, t_env **env, t_env **undefined)
{
	char	*name_to_add;
	char	*value;
	t_env	*tmp;

	tmp = *env;
	name_to_add = ft_get_name(str);
	value = ft_get_value(str);
	while (tmp)
	{
		if (!ft_strncmp(name_to_add,
				tmp->name, ft_max_strlen(name_to_add, tmp->name)))
		{
			if (value)
				ft_change_value(str, &tmp);
			ft_safe_free((void **)&name_to_add);
			return (1);
		}
		tmp = tmp->next;
	}
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
	ft_safe_free((void **)&name_to_add);
	return (0);
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
		while ((tmp->str)[i])
		{
			if ((tmp->str)[i] && (tmp->str)[i] == ')' && ((!(tmp->str)[i - 1]
								|| (tmp->str)[i - 1] != '\\')))
				c_brckt++;
			i++;
		}
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

int	ft_last_check(t_token *tokens)
{
	t_token	*tmp;
	int		o_brckt;
	int		i;
	int		res;

	tmp = tokens;
	o_brckt = 0;
	i = 0;
	res = 0;
	while (tmp)
	{
		while ((tmp->str)[i])
		{
			if ((tmp->str)[i] && (tmp->str)[i] == '(' && ((!(tmp->str)[i - 1]
									|| (tmp->str)[i - 1] != '\\')))
				o_brckt++;
			i++;
		}
		if (o_brckt > 1)
		{
			res = ft_check_next(tokens, o_brckt);
			if (res == 0)
				return (0);
			else if (res == 2)
			{
				if (!ft_quit_brckt_dup(tmp, '('))
					return (0);
			}
		}
		o_brckt = 0;
		i = 0;
		tmp = tmp->next;
	}
	return (1);
}

int	ft_check_brackets(t_token *token)
{
	char	*value;
	int		i;
	int		open;
	int		close;

	value = token->str;
	i = 0;
	open = 0;
	close = 0;
	while (value[i] && (value[i] == '('
			&& (!value[i - 1] || value[i - 1] != '\\')))
	{
		i++;
		open++;
	}
	while (value[i] && (value[i] != '(' && value[i] != ')'))
		i++;
	if (value[i] && value[i] == '(' && (!value[i - 1] || value[i - 1] != '\\'))
		return (ft_printf("syntax error near unsexpected token blabla\n"), 0);
	while (value[i] && (value[i] == ')'
			&& (!value[i - 1] || value[i - 1] != '\\')))
	{
		i++;
		close++;
	}
	while (value[i] && value[i] == ' ')
		i++;
	if (close && value[i])
		return (ft_printf("syntax error near unsexpected token blabla\n"), 0);
	else if (open && close)
		ft_quit_brackets(token, &open, &close);
	return (1);
}

int	ft_is_builtin(char *input)
{
	if (*input == '(')
		input = input + 1;
	if (!ft_strncmp(input, "echo", 4))
		return (1);
	else if (!ft_strncmp(input, "cd", 2))
		return (1);
	else if (!ft_strncmp(input, "pwd", 3))
		return (1);
	else if (!ft_strncmp(input, "export", 6))
		return (1);
	else if (!ft_strncmp(input, "unset", 5))
		return (1);
	else if (!ft_strncmp(input, "env", 3))
		return (1);
	else if (!ft_strncmp(input, "exit", 4))
		return (1);
	else
		return (0);
}
