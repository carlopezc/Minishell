/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenization.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:14:58 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/23 13:31:58 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_next_word(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] == ' ')
		i++;
	if (str[i] && !ft_check_operator(&str[i]))
		return (1);
	return (0);
}

char	*ft_group_input(char *input, int *i)
{
	char	*value;
	t_quote	q;

	value = NULL;
	ft_init_quote(&q);
	while (input[*i])
	{
		if (!q.flag)
		{
			if (input[*i] && input[*i]== ' ' && ft_next_word(&input[*i]))
				value = ft_strjoin_char(value, input[(*i)++]);
			while (input[*i] && input[*i] == ' ')
				(*i)++;
			if (ft_check_operator(&input[*i]))
				return (value);
		}
		if ((input[*i] == '\'' || input[*i] == '\"') && input[*i - 1] != '\\')
			ft_check_quote(&q, input[*i]);
		value = ft_strjoin_char(value, input[(*i)++]);
	}
	return (value);
}

int	ft_define_parts(char *input, char **value, t_token_type *type, int *i)
{
	if (input[*i] == '.' && input[*i + 1] && input[*i + 1] == '/')
	{
		*type = EXEC;
		*value = ft_group_input(input, i);
		return (1);
	}
	else if (ft_is_builtin(&input[*i]))
	{
		*type = BUILTIN;
		*value = ft_group_input(input, i);
		return (1);
	}
	*type = ft_is_operator(value, input, i);
	if (*type == ERROR)
		return (0);
	else if (*type != NOT_SET)
		return (1);
	else
	{
		*type = COMMAND;
		*value = ft_group_input(input, i);
		return (1);
	}
}

int	ft_group_command(t_minishell **minishell, char *input, int *i)
{
	t_token_type	type;
	char			*value;
	t_token			*token;

	token = NULL;
	value = NULL;
	type = NOT_SET;
	if (!ft_define_parts(input, &value, &type, i))
		return (0);
	token = ft_create_token(value, type);
	if (!token)
		return (0);
	ft_add_node_back(&((*minishell)->tokens), token);
	return (1);
}

int	ft_process_input(t_minishell **minishell, char *input)
{
	int	i;
	char	*final_input;

	i = 0;
	final_input = NULL;
	if (!minishell || !input)
		return (0);
	final_input = ft_parsing(input, minishell);
	ft_safe_free((void **)&input);
	if (!final_input)
		return (0);
	while (final_input[i])
	{
		while (final_input[i] && final_input[i] == ' ')
			i++;
		if (!ft_group_command(minishell, final_input, &i))
			return (ft_safe_free((void **)&final_input), 0);
	}
	ft_safe_free((void **)&final_input);
	return (1);
}
