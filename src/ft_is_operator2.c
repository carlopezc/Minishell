/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_operator2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 16:57:48 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/25 14:46:13 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

t_token_type	ft_is_operator4(char **value,
		char *input, int *i, int flag)
{
	if (!ft_strncmp(input + *i + flag, "|", 1))
	{
		if (flag)
			return (ft_printf("syntax error\n"), ERROR);
		*value = ft_strdup("|");
		*i += 1;
		return (PIPE);
	}
	else if (!ft_strncmp(input + *i + flag, "&&", 2))
	{
		if (flag)
			return (ft_printf("syntax error\n"), ERROR);
		*value = ft_strdup("&&");
		*i += 2;
		return (AND);
	}
	return (NOT_SET);
}

t_token_type	ft_handle_redirect_append(
	char **value, char *input, int *i, int flag)
{
	if (!ft_strncmp(input + *i + flag, ">>", 2))
	{
		*i += 2;
		if (flag)
		{
			(*i)++;
			if (input[*i] == ')' || !ft_check_error(input, *i, ')'))
			{
				ft_printf("syntax error\n");
				return (ERROR);
			}
		}
		ft_safe_free((void **)value);
		*value = ft_get_next(input, i);
		if (!ft_strncmp(*value, "", ft_strlen(*value)))
		{
			ft_safe_free((void **)value);
			return (ERROR);
		}
		return (APPEND);
	}
	return (NOT_SET);
}

t_token_type	ft_handle_redirect_out(
	char **value, char *input, int *i, int flag)
{
	if (!ft_strncmp(input + *i + flag, ">", 1))
	{
		*i += 1;
		if (flag)
		{
			(*i)++;
			if (input[*i] == ')' || !ft_check_error(input, *i, ')'))
			{
				ft_printf("syntax error\n");
				return (ERROR);
			}
		}
		ft_safe_free((void **)value);
		*value = ft_get_next(input, i);
		if (!ft_strncmp(*value, "", ft_strlen(*value)))
		{
			ft_safe_free((void **)value);
			return (ERROR);
		}
		return (REDIR_OUT);
	}
	return (NOT_SET);
}

t_token_type	ft_handle_pipe_or(
	char **value, char *input, int *i, int flag)
{
	if (!ft_strncmp(input + *i + flag, "||", 2))
	{
		if (flag)
		{
			ft_printf("syntax error\n");
			return (ERROR);
		}
		*i += 2;
		if (*(input + *i + flag) == ')')
		{
			*value = ft_strdup("||)");
			(*i)++;
		}
		else
			*value = ft_strdup("||");
		return (OR);
	}
	return (NOT_SET);
}

t_token_type	ft_handle_redirect_in(
	char **value, char *input, int *i, int flag)
{
	if (!ft_strncmp(input + *i + flag, "<", 1))
	{
		*i += 1;
		if (flag)
		{
			(*i)++;
			if (input[*i] == ')' || !ft_check_error(input, *i, ')'))
			{
				ft_printf("syntax error\n");
				return (ERROR);
			}
		}
		ft_safe_free((void **)value);
		*value = ft_get_next(input, i);
		if (!ft_strncmp(*value, "", ft_strlen(*value)))
		{
			ft_safe_free((void **)value);
			return (ERROR);
		}
		return (REDIR_IN);
	}
	return (NOT_SET);
}
