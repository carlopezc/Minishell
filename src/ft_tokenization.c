/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenization.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:14:58 by carlopez          #+#    #+#             */
/*   Updated: 2025/04/02 18:03:22 by carlotalcd       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../header/ft_minishell.h"

char	*token_type_to_str(t_token_type type)
{
	if (type == COMMAND)
		return ("COMMAND");
	if (type == PIPE)
		return ("PIPE");
	if (type == EXEC)
		return ("EXEC");
	if (type == BUILTIN)
		return ("BUILTIN");
	if (type == REDIR_IN)
		return ("REDIR_IN");
	if (type == REDIR_OUT)
		return ("REDIR_OUT");
	if (type == HEREDOC)
		return ("HEREDOC");
	if (type == APPEND)
		return ("APPEND");
	if (type == AND)
		return ("AND");
	if (type == OR)
		return ("OR");
	if (type == NOT_SET)
		return ("NOT_SET");
	return (NULL);
}

void	ft_add_node_back(t_token **lst, t_token *new)
{
	t_token	*temp;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
	return ;
}

t_token	*ft_create_node(char *str, t_token_type type)
{
	t_token	*token;

	if (!str)
		return (NULL);
	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->str = str;
	token->type = type;
	token->next = NULL;
	return (token);
}

void	ft_free_tokens(t_token **tokens)
{
	t_token 	*tmp;

	while (tokens && *tokens)
	{
		tmp = (*tokens)->next;
		if ((*tokens)->str)
			free((*tokens)->str);
		free(*tokens);
		*tokens = tmp;
	}
	return ;
}


void	ft_expand(char **input, char **env)
{
	int 	i;
	char	*name_var;
	char	*final_var;

	i = 0;
	if (!input || !*input || !env || !*env)
		return ;
	name_var = ft_substr(*input, 1, ft_strlen(*input));
	if (!name_var)
		return ;
	while (env[i])
	{
		if (!ft_strncmp(name_var, env[i], ft_strlen(name_var)) && *(env[i] + ft_strlen(name_var)) == '=')
		{
			final_var = ft_substr(env[i], ft_strlen(name_var) + 1, ft_strlen(env[i]) - ft_strlen(name_var) - 1);
			if (!final_var)
				return ;
			*input = final_var;
			return (free(name_var));
		}
		i++;
	}
	*input = NULL;
	return (free(name_var));
}
char	*ft_check_var(t_minishell *minishell, char *s_input)
{
	/*
	if (!ft_strncmp(s_input, "$?", ft_strlen(s_input)))
	{
		//Funcion que imprima el exit status
		return (//Funcion que imprima el exit status)
	}
		*/
	if (!ft_strncmp(s_input, "$", 1))
		ft_expand(&s_input, minishell->env);
	return (s_input);
}

char	*ft_group_input(t_minishell *minishell, int *i)
{
	char	*input;
	char	*tmp;

	input = ft_check_var(minishell, minishell->s_input[*i]);
	while (input && minishell->s_input[*i + 1])
	{
		//este me controla tb el ||
		if (minishell->s_input[*i + 1] && !ft_strncmp(minishell->s_input[*i + 1], "||", 3))
			return (input);
		else if (minishell->s_input[*i + 1] && !ft_strncmp(minishell->s_input[*i + 1], "|", 2))
			return (input);
		else if (minishell->s_input[*i + 1] && !ft_strncmp(minishell->s_input[*i + 1], "<", 2))
			return (input);
		else if (minishell->s_input[*i + 1] && !ft_strncmp(minishell->s_input[*i + 1], ">", 2))
			return (input);
		else if (minishell->s_input[*i + 1] && !ft_strncmp(minishell->s_input[*i + 1], ">>", 3))
			return (input);
		else if (minishell->s_input[*i + 1] && !ft_strncmp(minishell->s_input[*i + 1], "<<", 3))
			return (input);
		else if (minishell->s_input[*i + 1] && !ft_strncmp(minishell->s_input[*i + 1], "&&", 3))
			return (input);
		else
		{
			tmp = ft_strjoin(input, " ");
			input = ft_check_var(minishell, minishell->s_input[++(*i)]);
			if (!input)
				input = "";
			input = ft_strjoin(tmp, input);
			free(tmp);
		}
	}
	return (input);
}

t_token_type	ft_is_operator(char *input)
{
 	if (!ft_strncmp(input, "||", 2))
		return (OR);
	else if (!ft_strncmp(input, "<<", 2))
		return (APPEND);
	else if (!ft_strncmp(input, ">>", 2))
		return (HEREDOC);
	else if (!ft_strncmp(input, ">", 1))
		return (REDIR_OUT);
	else if (!ft_strncmp(input, "<", 1))
		return (REDIR_IN);
	else if (!ft_strncmp(input, "|", 1))
		return (PIPE);
	else if (!ft_strncmp(input, "&&", 2))
		return (AND);
	else
		return (NOT_SET);
}

int	ft_is_builtin(char *input, char *next)
{
	if (!ft_strncmp(input, "echo", ft_strlen(input)))
		return (1);
	else if (!ft_strncmp(input, "cd", ft_strlen(input)))
		return (1);
	else if (!ft_strncmp(input, "pwd", ft_strlen(input)))
	{
		//si pasa esto habria que meter un exit, o no se si lo gestionamos aqui o en el execve
		if (next)
			ft_printf("pwd: too many arguments\n");
		return (1);
	}
	else if (!ft_strncmp(input, "export", ft_strlen(input)))
		return (1);
	else if (!ft_strncmp(input, "unset", ft_strlen(input)))
		return (1);
	else if (!ft_strncmp(input, "env", ft_strlen(input)))
		return (1);
	else if (!ft_strncmp(input, "exit", ft_strlen(input)))
		return (1);
	else
		return (0);
}

int	ft_define_parts(t_minishell **minishell, char **input, t_token_type *type, int *i)
{
	if (!ft_strncmp((*minishell)->s_input[*i], "./", 2))
	{
		*type = EXEC;
		*input = ft_group_input(*minishell, i);
		return (1);
	}
	else if (ft_is_builtin((*minishell)->s_input[*i], (*minishell)->s_input[*i + 1]))
	{
		*type = BUILTIN;
		*input = ft_group_input(*minishell, i);
		return (1);
	}
	*type = ft_is_operator((*minishell)->s_input[*i]);
	if (*type != NOT_SET)
	{
		//tendre que controlar que me pueden pasar el archivo pegado, tipo >hola
		if (*type == PIPE || *type == OR || *type == AND)
			*input = (*minishell)->s_input[*i];
		else
		{
			if ((*minishell)->s_input[*i + 1])
				*input = (*minishell)->s_input[++(*i)];
			else
				return (0);
		}
		return (1);
	}
	else
	{
		*type = COMMAND;
		*input = ft_group_input(*minishell, i);
		if (!*input)
			return (0);
		return (1);
	}
	return (0);
}

int	ft_group_command(t_minishell **minishell, int *i)
{
	t_token_type	type;
	char	*input;
	t_token	*token;

	token = NULL;
	input = NULL;
	type = NOT_SET;
	if (!ft_define_parts(minishell, &input, &type, i))
		return (0);
	token = ft_create_node(input, type);
	if (!token)
		return (0);
	ft_add_node_back(&((*minishell)->tokens), token);
	return (1);
}

int	ft_process_input(t_minishell **minishell, char *input)
{
	char	**s_input;
	int	i;

	i = 0;
	if (!minishell | !input)
		return (0);
	s_input = ft_split(input, ' ');
	if (!s_input)
		return (0);
	(*minishell)->s_input = s_input;
	while ((*minishell)->s_input && (*minishell)->s_input[i])
	{
		if (!ft_group_command(minishell, &i))
			return (0);
		i++;
	}
	return (1);
}

void	ft_free_array(char **arr)
{
	int	i;

	if (!arr || !*arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

char	**ft_strdup_env(char **env)
{
	int	i;
	char	**cpy_env;

	i = 0;
	if (!env || !*env)
		return (NULL);
	while (env[i])
		i++;
	cpy_env = (char **)malloc((i + 1) * sizeof(char *));
	if (!cpy_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		cpy_env[i] = ft_strdup(env[i]);
		if (!cpy_env[i])
			return (ft_free_array(cpy_env), NULL);
		i++;
	}
	cpy_env[i] = NULL;
	return (cpy_env);
}

void	ft_add_to_env(t_minishell **minishell, char *str)
{
	int	i;
	char	**cpy;
	char	**env;

	i = 0;
	env = (*minishell)->env;
	if (!env || !*env || !str)
		return ;
	while (env[i])
		i++;
	cpy = (char **)malloc((i + 2) * sizeof(char *));
	if (!cpy)
		return ;
	i = 0;
	while (env[i])
	{
		cpy[i] = ft_strdup(env[i]);
		if (!cpy[i])
			return (ft_free_array(cpy));
		i++;
	}
	cpy[i++] = str;
	cpy[i] = NULL;
	ft_free_array(env);
	(*minishell)->env = cpy;
	return ;
}
char	*ft_quote_string(char *str)
{
	int 	i;
	int		j;
	char	*quoted_str;

	i = 0;
	if (!str)
		return (NULL);
	i = ft_strlen(str);
	quoted_str = (char *)malloc((i + 3) * sizeof(char));
	if (!quoted_str)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i] && str[i] != '=')
		quoted_str[j++] = str[i++];
	if (str[i] && str[i] == '=')
		quoted_str[j++] = str[i++];
	quoted_str[j++] = '"';
	while (str[i])
		quoted_str[j++] = str[i++];
	quoted_str[j++] = '"';
	quoted_str[j] = '\0';
	return (quoted_str);
}

char	**ft_add_quotes(char **export)
{
	char	**quoted_export;
	int		j;

	j = 0;
	if (!export || !*export)
		return (NULL);
	while (export[j])
		j++;
	quoted_export = (char **)malloc((j + 1) * sizeof(char *));
	if (!quoted_export)
		return (NULL);
	j = 0;
	while (export[j])
	{
		quoted_export[j] = ft_quote_string(export[j]);
		if (!quoted_export[j])
			return (ft_free_array(export), NULL);
		j++;
	}
	quoted_export[j] = NULL;
	return (quoted_export);
}

char	**ft_create_export(char **export)
{
	int 	i;
	int		j;
	int swapped;
	char	*tmp;

	i = 0;
	j = 0;
	swapped = 1;
	if (!export || !*export)
		return (NULL); //Caso especial que ha dicho luqui no se que habra que hacer
	while (export[j])
		j++;
	while (swapped)
	{
		swapped = 0;
		i = 0;
		while (i < j - 1)
		{
			if (ft_strncmp(export[i], export[i + 1], ft_strlen(export[i])) > 0)
			{
				tmp = export[i];
				export[i] = export[i + 1];
				export[i + 1] = tmp;
				swapped = 1;
			}
			i++;
		}
	}
	return (ft_add_quotes(export));
}

int	ft_init_minishell(t_minishell **minishell, char **env)
{
	*minishell = (t_minishell *)malloc(sizeof(t_minishell));
	if (!(*minishell))
		return (0);
	(*minishell)->tokens = NULL;
	(*minishell)->env = ft_strdup_env(env);
	(*minishell)->export = ft_create_export(ft_strdup_env(env));
	(*minishell)->s_input = NULL;
	return (1);
}