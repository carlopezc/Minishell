/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenization.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:14:58 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/29 17:51:28 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../header/ft_minishell.h"

char	*ft_strjoin_char(char *str, char c)
{
	int	i;
	char *final;

	if (!str || !*str)
		i = 0;
	else
		i = ft_strlen(str);
	final = (char *)malloc((i + 2) * sizeof(char));
	if (!final)
		return (NULL);
	i = 0;
	while (str && str[i])
	{
		final[i] = str[i];
		i++;
	}
	final[i++] = c;
	final[i] = '\0';
	free(str);
	return (final);
}

char	*ft_group_input(t_minishell **minishell, char *input, int *i)
{
	char	*value;

	value = NULL;
	(void)minishell;
	while (input[*i])
	{
		if (ft_check_operator(&input[*i]))
			return (value);
		value = ft_strjoin_char(value, input[*i]);
		(*i)++;
	}
	return (value);
}


int	ft_is_builtin(char *input)
{
	//Si pwd tiene argumentos error, creo que lo gestiona lucas
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

int	ft_define_parts(t_minishell **minishell, char *input, char **value, t_token_type *type, int *i)
{
	if (input[*i] == '.' && input[*i + 1] && input[*i + 1] == '/')
	{
		*type = EXEC;
		*value = ft_group_input(minishell, input, i);
		return (1);
	}
	else if (ft_is_builtin(&input[*i]))
	{
		*type = BUILTIN;
		*value = ft_group_input(minishell, input, i);
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
		*value = ft_group_input(minishell, input, i);
		return (1);
	}
}

int	ft_group_command(t_minishell **minishell, char *input, int *i)
{
	t_token_type	type;
	char	*value;
	t_token	*token;

	token = NULL;
	value = NULL;
	type = NOT_SET;

	if (!ft_define_parts(minishell, input, &value, &type, i))
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
	if (!ft_parsing(&input, minishell))
		return (0);
	while (input[i])
	{
		while (input[i] && input[i] == ' ')
			i++;
		if (!ft_group_command(minishell, input, &i))
			return (0);
	}
	return (1);
}

void	ft_change_value(char *str, t_env **node)
{
	char	*equal;

	equal = ft_strchr(str, '=');
	if (*(equal - 1) == '+') 
		(*node)->value = ft_strjoin((*node)->value, ft_get_value(str));
	else
	{
		ft_safe_free((void **)&((*node)->value));
		(*node)->value = ft_get_value(str);
	}
	return ;
}

int	ft_check_duplicated(char *str, t_env **env, t_env **undefined)
{
	char	*name_to_add;
	char	*value;
	t_env *tmp;

	tmp = *env;
	name_to_add = ft_get_name(str);
	value = ft_get_value(str);
	while (tmp)
	{
		if (!ft_strncmp(name_to_add, tmp->name, ft_max_strlen(name_to_add, tmp->name)))
		{
			if (value)
				ft_change_value(str, &tmp);
			ft_safe_free((void **)&name_to_add);
			ft_safe_free((void **)&value);
			return (1);
		}
		tmp = tmp->next;
	}
	if (undefined)
	{
		tmp = *undefined;
		while (tmp)
		{
			if (!ft_strncmp(name_to_add, tmp->name, ft_max_strlen(name_to_add, tmp->name)))
			{
				ft_free_node(tmp, undefined);
				ft_safe_free((void **)&name_to_add);
				return (0);
			}
			tmp = tmp->next;
		}
	}
	ft_safe_free((void **)&name_to_add);
	ft_safe_free((void **)&value);
	return (0);
}


