/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:05:18 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/17 18:39:42 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

t_token_type	ft_is_operator_aux_aux(char **value,
		char *input, int *i, int flag)
{
	if (!ft_strncmp(input + *i, "|", 1))
	{
		if (flag)
			return (ft_printf("syntax error near unexpected token\n"), ERROR);
		*value = ft_strdup("|");
		*i += 1;
		return (PIPE);
	}
	else if (!ft_strncmp(input + *i, "&&", 2))
	{
		if (flag)
			return (ft_printf("syntax error near unexpected token\n"), ERROR);
		*value = ft_strdup("&&");
		*i += 2;
		return (AND);
	}
	else if (flag)
		(*i)--;
	return (NOT_SET);
}

t_token_type	ft_is_operator_aux(char **value, char *input, int *i, int flag)
{
	if (!ft_strncmp(input + *i, ">>", 2))
	{
		*i += 2;
		if (flag)
		{
			if (*(input + *i) == ')')
				return (ft_printf("syntax error near unexpected token\n"), ERROR);
			*value = ft_strjoin("(", *value);
		}
		*value = ft_get_next(input, i);
		if (!ft_strncmp(*value, "", ft_strlen(*value)))
			return (ERROR);
		return (APPEND);
	}
	else if (!ft_strncmp(input + *i, ">", 1))
	{
		*i += 1;
		if (flag)
		{
			if (*(input + *i) == ')')
				return (ft_printf("syntax error near unexpected token\n"), ERROR);
			*value = ft_strjoin("(", *value);
		}
		*value = ft_get_next(input, i);
		if (!ft_strncmp(*value, "", ft_strlen(*value)))
			return (ERROR);
		return (REDIR_OUT);
	}
	else if (!ft_strncmp(input + *i, "<", 1))
	{
		*i += 1;
		if (flag)
		{
			if (*(input + *i) == ')')
				return (ft_printf("syntax error near unexpected token\n"), ERROR);
			*value = ft_strjoin("(", *value);
		}
		*value = ft_get_next(input, i);
		if (!ft_strncmp(*value, "", ft_strlen(*value)))
			return (ERROR);
		return (REDIR_IN);
	}
	return (ft_is_operator_aux_aux(value, input, i, flag));
}

t_token_type	ft_is_operator(char **value, char *input, int *i)
{
	int	flag;

	flag = 0;
	if (*(input + *i) == '(')
	{
		flag = 1;
		(*i)++;
	}
	if (!ft_strncmp(input + *i, "||", 2))
	{
		if (flag)
			return (ft_printf("syntax error near unexpected token\n"), ERROR);
		else
		{
			*value = ft_strdup("||");
			*i += 2;
		}
		return (OR);
	}
	else if (!ft_strncmp(input + *i, "<<", 2))
	{
		*i += 2;
		if (flag)
		{
			if (*(input + *i) == ')')
				return (ft_printf("syntax error near unexpected token\n"), ERROR);
			*value = ft_strjoin("(", *value);
		}
		*value = ft_get_next(input, i);
		if (!ft_strncmp(*value, "", ft_strlen(*value)))
			return (ERROR);
		return (HEREDOC);
	}
	return (ft_is_operator_aux(value, input, i, flag));
}

void	ft_check_quote(t_quote *quote, char c)
{
	if (c == '\'' || c == '\"')
	{
		if (quote->flag == 0)
		{
			quote->flag = 1;
			quote->type = c;
		}
		else if (quote->type != c)
			return ;
		else if (quote->type == c)
		{
			quote->flag = 0;
			quote->type = 0;
		}
	}
	return ;
}

int	ft_check_operator(char *input)
{
	if (!input || input[0] == '\0')
		return (0);
	else if (*input == '(' || *input == ')')
		input = input + 1;
	if (!ft_strncmp(input, "||", 2) || !ft_strncmp(input, "|", 1))
		return (1);
	else if (!ft_strncmp(input, ">>", 2) || !ft_strncmp(input, ">", 1))
		return (1);
	else if (!ft_strncmp(input, "<<", 2) || !ft_strncmp(input, "<", 1))
		return (1);
	else if (!ft_strncmp(input, "&&", 2))
		return (1);
	else
		return (0);
}
