/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenization.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:14:58 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/21 12:07:05 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../header/ft_minishell.h"

void	ft_print_tokens(t_token	*token);

char	*token_type_to_str(t_token_type type)
{
	if (type == COMMAND)
		return ("COMMAND");
	if (type == O_BRACKET)
		return ("O_BRACKET");
	if (type == C_BRACKET)
		return ("C_BRACKET");
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
	while (temp->next)
		temp = temp->next;
	temp->next = new;
	return ;
}

t_token	*ft_create_token(char *str, t_token_type type)
{
	t_token	*token;

	if (!str)
		return (NULL);
	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->str = ft_strdup(str);
	token->type = type;
	token->next = NULL;
	return (token);
}

void	ft_free_tokens(t_minishell **minishell)
{
	t_token 	*tmp;
	t_token 	*token;

	token = (*minishell)->tokens;
	tmp = NULL;
	while (token)
	{
		tmp = token->next;
		if (token->str)
			token->str = NULL;
		free(token);
		token = tmp;
	}
	return ;
}

char	*ft_expand(char *input, int *i, t_env *env)
{
	char	*name_var;
	t_env	*tmp;
	int start;

	if (!input || !env)
		return (NULL);
	start = *i;
	if (!ft_isalpha(input[*i]) && (input[*i] != '_'))
		return (ft_printf("Non valid name\n"), ft_strdup(""));
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	if (*i == start)
		return (ft_strdup("$"));
	name_var = ft_substr(input, start, *i - start);
	if (input[*i] == '\'' || input[*i] == '"')
		(*i)--;
	if (!name_var)
		return (NULL);
	tmp = env;
	while (tmp)
	{
		if (!ft_strncmp(name_var, tmp->name, ft_max_strlen(tmp->name, name_var)))
		{
			free(name_var);
			//Falla en algunos casos comiendose un caracter o anadiendolo
			//(*i)--;
			return (ft_strdup(tmp->value));
		}
		tmp = tmp->next;
	}
	free(name_var);
	return (ft_strdup(""));
}
/*
char	*ft_check_var(t_minishell *minishell, char *input, int *i)
{
	if (!input || input[*i] != '$')
		return (NULL);
	(*i)++;
	if (input[*i] == '?')
	{
		(*i)++;
		ft_printf("Aqui ira el codigo de salida\n");
		return (ft_strdup("0"));
	}
	return (ft_expand(input, i, minishell->env));
}
*/
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

void	ft_check_quote(t_quote *quote, char c, int *i)
{
	(void)*i;
	if (c == '\'' || c == '\"')
	{
		if (quote->flag == 0)
		{
			quote->flag = 1;
			quote->type = c;
		}
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

char	*ft_get_next(char *input, int *i)
{
	int start;

	while (input[*i] && input[*i] == ' ')
		(*i)++;
	start = *i;
	while (input[*i] && input[*i] != ' ' && input[*i] != '|'
		&& input[*i] != '<' && input[*i] != '>' && input[*i] != '&')
		(*i)++;
	return (ft_substr(input, start, *i - start));
}

t_token_type	ft_is_operator(char **value, char *input, int *i)
{
 	if (!ft_strncmp(input + *i, "||", 2))
	{
		*value = ft_strdup("||");
		*i += 2;
		return (OR);
	}
	else if (!ft_strncmp(input + *i, "<<", 2))
	{
		*i += 2;
		*value = ft_get_next(input, i);
		return (HEREDOC);
	}
	else if (!ft_strncmp(input + *i, ">>", 2))
	{
		*i += 2;
		*value = ft_get_next(input, i); //coge el valor siguiente 
		return (APPEND);
	}
	else if (!ft_strncmp(input + *i, ">", 1))
	{
		*i += 1;
		*value = ft_get_next(input, i); //coge el valor siguiente
		return (REDIR_OUT);
	}
	else if (!ft_strncmp(input + *i, "<", 1))
	{
		*i += 1;
		*value = ft_get_next(input, i); //coge el valor siguiente
		return (REDIR_IN);
	}
	else if (!ft_strncmp(input + *i, "|", 1))
	{
		*value = ft_strdup("|");
		*i += 1;
		return (PIPE);
	}
	else if (!ft_strncmp(input + *i, "&&", 2))
	{
		*value = ft_strdup("&&");
		*i += 2;
		return (AND);
	}
	else
		return (NOT_SET);
}

int	ft_is_builtin(char *input)
{
	//Si pwd tiene argumentos error
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
	//luego miro el is builtin
	*type = ft_is_operator(value, input, i);
	if (*type != NOT_SET)
		return (1); //Ya ha avanzado el puntero en caso de que sea operador
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

	//de momento define parts nunca devuelve 0
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
	ft_printf("\n TRAS PARSING\n");
	ft_printf("%s\n", input);
	while (input[i])
	{
		while (input[i] && input[i] == ' ')
			i++;
		if (!ft_group_command(minishell, input, &i))
			return (0);
	}
	return (1);
}

void	ft_free_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		ft_safe_free((void **)&arr[i++]);
	free(arr);
	return ;
}

int	ft_arraylen(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return (i);
	while (arr[i])
		i++;
	return (i);
}

t_env	*ft_strdup_env(t_env *env)
{
	t_env	*cpy;
	t_env *node;
	t_env	*tmp;

	if (!env)
		return (NULL);
	cpy = NULL;
	tmp = env;
	while (tmp)
	{
		node = ft_create_node(ft_strdup(tmp->name), ft_strdup(tmp->value));
		if (!node)
			return (ft_free_env(&cpy), NULL);
		ft_connect_node(&cpy, node);
		tmp = tmp->next;
	}
	return (cpy);
}

char	*ft_get_name(char *str)
{
	int	i;
	char	*name;

	i = 0;
	if (!str)
		return (NULL);
	while (str && str[i])
	{
		if (str[i] == '=')
			break ;
		else if (str[i] == '+' && str[i + 1] && str[i + 1] == '=')
			break ;
		i++;
	}
	name = ft_substr(str, 0, i);
	return (name);
}

char	*ft_get_value(char *str)
{
	int 	i;
	char	*value;
	char	*equal;

	i = 0;
	value = NULL;
	equal = ft_strchr(str, '=');
	if (!equal)
		return (NULL);
	else if (!*(equal + 1))
		value = ft_strdup("");
	else
	{
		i++;
		while (equal[i])
			value = ft_strjoin_char(value, equal[i++]);
	}
	if ((value[0] == '\"' || value[0] == '\'') && ((value[ft_strlen(value) - 1] == '\"') || (value[ft_strlen(value) - 1] == '\'')))
		value = ft_substr(value, 1, ft_strlen(value) - 2);
	return (value);
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

void	ft_free_node(t_env *node, t_env **list)
{
	t_env *tmp;
	t_env *prev;

	tmp = *list;
	prev = NULL;
	if (!node)
		return ;
	if (tmp == *list && tmp->next == NULL)
	{
		ft_safe_free((void **)&tmp->name);
		ft_safe_free((void **)&tmp->value);
		ft_safe_free((void **)&tmp);
		*list = NULL;
		list = NULL;
		return ;
	}
	while (tmp)
	{
		if (tmp->next && !ft_strncmp((tmp->next)->name,
				node->name, ft_max_strlen((tmp->next)->name, node->name)))
			prev = tmp;
		if (!ft_strncmp(tmp->name, node->name, ft_max_strlen(tmp->name, node->name)))
		{
			if (prev)
				prev->next = tmp->next;
			else
				*list = tmp->next;
			ft_safe_free((void **)&tmp->name);
			ft_safe_free((void **)&tmp->value);
			tmp->next = NULL;
			ft_safe_free((void **)&tmp);
			return ;
		}
		tmp = tmp->next;
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
			return (1);
		}
		tmp = tmp->next;
	}
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
	ft_safe_free((void **)&name_to_add);
	return (0);
}

t_env	*ft_create_node(char *name, char *value)
{
	t_env *node;

	if (!name)
		return (NULL);
	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->name = name;
	if (!value)
		node->value = NULL;
	else
		node->value = value;
	node->next = NULL;
	return (node);
}

void	ft_connect_token(t_token **tokens, t_token *new, t_token *next)
{
	t_token *tmp;
	t_token *save;

	if (!tokens)
		return ;
	save = NULL;
	tmp = *tokens;
	if (tmp == next)
	{
		save = *tokens;
		*tokens = new;
		new->next = save;
		return ;
	}
	while (tmp && tmp->next)
	{
		if (tmp->next == next)
		{
			save = tmp->next;
			tmp->next = new;
			new->next = save;
			return ;
		}
		tmp = tmp->next;
	}
	if (tmp && !tmp->next)
		tmp->next = new;
	return ;
}

void	ft_connect_node(t_env **env, t_env *node)
{
	t_env	*tmp;

	if (!*env)
	{
		*env = node;
		return ;
	}
	tmp = *env;
	while (tmp && tmp->next)
		tmp = tmp->next;
	tmp->next = node;
	return ;
}

int	ft_listlen(t_env *env)
{
	int 	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}

char	**ft_create_array_env(t_env *env)
{
	int	i;
	char **env_array;
	char *tmp;

	if (!env)
		return (NULL);
	i = ft_listlen(env);
	env_array = (char **)malloc((i + 1) * sizeof(char *));
	if (!env_array)
		return (NULL);
	env_array[i] = NULL;
	i = 0;
	while (env)
	{
		if (env->name && env->value)
		{
			tmp = ft_strjoin(env->name, "=");
			env_array[i++] = ft_strjoin(tmp, env->value);
			ft_safe_free((void **)&tmp);
		}
		else
			env_array[i++] = env->name;
		env = env->next;
	}
	return (env_array);
}

t_env	*ft_create_env(char **env_array)
{
	t_env *env;
	t_env *node;
	char *name;
	char *value;
	int	i;

	if (!env_array || !*env_array)
		return (NULL);
	i = 0;
	env = NULL;
	while (env_array[i])
	{
		name = ft_get_name(env_array[i]);
		if (!ft_strncmp(name, "SHLVL", ft_max_strlen(name, "SHLVL")))
			value = ft_itoa(ft_atoi(ft_get_value(env_array[i])) + 1);
		else
			value = ft_get_value(env_array[i]);
		node = ft_create_node(ft_strdup(name), ft_strdup(value));
		if (!node)
			return (ft_printf("Error creating environment node\n"), NULL);
		ft_connect_node(&env, node);
		i++;
	}
	return (env);
}

void	ft_empty_export(t_minishell **shell)
{
	t_env *node;

	node = ft_create_node(ft_strdup("OLDPWD"), NULL);
	if (!node)
		return ;
	ft_connect_node(&(*shell)->undefined_var, node);
	ft_merge_lists(shell, (*shell)->env, (*shell)->undefined_var);
	return ;
}

t_env	*ft_empty_env(void)
{
	t_env *env;
	t_env *node;
	char *cwd;

	env = NULL;
	node = ft_create_node(ft_strdup("SHLVL"), ft_strdup("1"));
	if (!node)
		return (NULL);
	ft_connect_node(&env, node);
	cwd = getcwd(NULL, 0);
	node = ft_create_node(ft_strdup("PWD"), ft_strdup(cwd));
	if (!node)
		return (ft_safe_free((void **)&cwd), NULL);
	ft_safe_free((void **)&cwd);
	ft_connect_node(&env, node);
	//Guarda ultimo comando o su ruta, cada vez que ejecutamos un comando habria que actualizarla
	//Podriamos antes de hacer el execve guardar la ruta del comandoen la var _
	node = ft_create_node(ft_strdup("_"), ft_strdup("./minishell"));
	if (!node)
		return (NULL);
	ft_connect_node(&env, node);
	return (env);
}

int	ft_init_minishell(t_minishell **minishell, char **env)
{
	*minishell = (t_minishell *)malloc(sizeof(t_minishell));
	if (!(*minishell))
		return (0);
	(*minishell)->tokens = NULL;
	(*minishell)->env_tmp = NULL;
	(*minishell)->undefined_var = NULL;
	(*minishell)->export = NULL;
	if (env && *env)
	{
		(*minishell)->env = ft_create_env(env);
		if (!(*minishell)->env)
			return (free(*minishell), ft_printf("Error creating environment \n"), 0);
		(*minishell)->export = ft_create_env(env);
	}
	else
	{
		(*minishell)->env = ft_empty_env();
		ft_empty_export(minishell);
	}
	if (!(*minishell)->export)
		return (ft_free_minishell(minishell), ft_printf("Error creating export \n"), 0);
	return (1);
}
