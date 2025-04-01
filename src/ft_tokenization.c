/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenization.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:14:58 by carlopez          #+#    #+#             */
/*   Updated: 2025/03/31 21:51:49 by carlotalcd       ###   ########.fr       */
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
	//no se como comprobar el token_type
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
char	*ft_check_var(char **input, t_minishell *minishell, char *s_input)
{
	if (!input || !*input)
		return (NULL);
	if (!ft_strncmp(s_input , "$", 1))
	{
		ft_expand(&s_input, minishell->env);
		*input = s_input;
	}
	return (s_input);
}

char	*ft_group_input(t_minishell *minishell, int *i)
{
	char	*input;
	char	*tmp;
	//char	*var;

	input = ft_check_var(&input, minishell, minishell->s_input[*i]);
	while (input && minishell->s_input[*i + 1])
	{
		//este me controla tb el ||
		if (minishell->s_input[*i + 1] && !ft_strncmp(minishell->s_input[*i + 1], "|", 2))
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
			input = ft_strjoin(tmp, ft_check_var(&input, minishell, minishell->s_input[++(*i)]));
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
		//si pasa esto habria que meter un exit
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
		return (1);
	}
	return (0);
}

int	ft_group_command(t_minishell **minishell, int *i)
{
	t_token_type	type;
	//char	**s_input;
	char	*input;
	t_token	*token;

	token = NULL;
	input = NULL;
	type = NOT_SET;
	ft_define_parts(minishell, &input, &type, i);
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

int	ft_init_minishell(t_minishell **minishell, char **env)
{
	*minishell = (t_minishell *)malloc(sizeof(t_minishell));
	if (!(*minishell))
		return (0);
	(*minishell)->tokens = NULL;
	(*minishell)->env = ft_strdup_env(env);
	(*minishell)->s_input = NULL;
	return (1);
}
