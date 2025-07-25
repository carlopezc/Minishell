/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 20:12:24 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/25 12:25:22 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_check_error(char *input, int i, int c)
{
	while (input[i] && input[i] == ' ')
		i++;
	if (!input[i] || input[i] == c)
		return (0);
	return (1);
}

t_token_type	ft_is_operator3(char **value, char *input, int *i, int flag)
{
	t_token_type	type;

	type = ft_handle_pipe_or(value, input, i, flag);
	if (type != NOT_SET)
		return (type);
	type = ft_handle_redirect_in(value, input, i, flag);
	if (type != NOT_SET)
		return (type);
	return (ft_is_operator4(value, input, i, flag));
}

t_token_type	ft_is_operator2(char **value, char *input, int *i, int flag)
{
	t_token_type	type;

	type = ft_handle_redirect_append(value, input, i, flag);
	if (type != NOT_SET)
		return (type);
	type = ft_handle_redirect_out(value, input, i, flag);
	if (type != NOT_SET)
		return (type);
	return (ft_is_operator3(value, input, i, flag));
}

void	ft_skip_spaces(char *input, int *i)
{
	if (!input)
		return ;
	while (input[*i] && input[*i] == ' ')
		(*i)++;
	return ;
}

t_token_type	ft_is_operator(char **value, char *input, int *i)
{
	int	flag;

	flag = 0;
	if (*(input + *i) == '(')
		flag = 1;
	ft_skip_spaces(input + flag, i);
	if (!ft_strncmp(input + *i + flag, "<<", 2))
	{
		*i += 2;
		if (flag)
			(*i)++;
		if (*(input + *i) == ')' || !ft_check_error(input, *i, ')'))
			return (ft_printf("syntax error\n"), ERROR);
		ft_safe_free((void **)value);
		*value = ft_get_next(input, i);
		if (!ft_strncmp(*value, "", ft_strlen(*value)))
			return (ft_safe_free((void **)value), ERROR);
		return (HEREDOC);
	}
	return (ft_is_operator2(value, input, i, flag));
}
