/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenization.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:14:58 by carlopez          #+#    #+#             */
/*   Updated: 2025/06/14 18:44:29 by carlotalcd       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

char	*ft_group_input(char *input, int *i)
{
	char	*value;

	value = NULL;
	while (input[*i])
	{
		if (ft_check_operator(&input[*i]))
			return (value);
		value = ft_strjoin_char(value, input[*i]);
		(*i)++;
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

	i = 0;
	if (!minishell || !input)
		return (0);
	ft_printf("input antes: %s\n", input);
	if (!ft_parsing(&input, minishell))
		return (0);
	while (input[i])
	{
		while (input[i] && input[i] == ' ')
			i++;
		if (!ft_group_command(minishell, input, &i))
			return (0);
	}
	ft_printf("input despues: %s\n", input);
	return (1);
}
