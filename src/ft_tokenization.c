/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenization.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:14:58 by carlopez          #+#    #+#             */
/*   Updated: 2025/04/17 20:43:20 by carlotalcd       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../header/ft_minishell.h"

void	ft_print_tokens(t_token	*token);

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
		//ft_printf("LIBERANDO CADENA , DIR DE MEMORIA: %p\n", (void *)token);
		if (token->str)
		{
			//ft_printf("liberando su str...\n");
			//ft_safe_free((void **)&token->str);
			token->str = NULL;
		}
		free(token);
		token = tmp;
	}
	return ;
}

void	ft_expand(char **input, t_env *env)
{
	char	*name_var;
	char	*final_var;
	t_env	*tmp;

	if (!input || !*input || !env)
		return ;
	name_var = ft_substr(*input, 1, ft_strlen(*input));
	if (!name_var)
		return ;
	tmp = env;
	while (tmp)
	{
		if (!ft_strncmp(name_var, tmp->name, ft_max_strlen(tmp->name, name_var)) && *(*input + ft_strlen(tmp->name)) == '=')
		{
			final_var = tmp->value;
			if (!final_var)
				return (free(name_var));
			free(name_var);
			free(*input);
			*input = ft_strdup(final_var);
			free(final_var);
			return ;
		}
		tmp = tmp->next;
	}
	*input = NULL;
	free(name_var);
	return ;
}
char	*ft_check_var(t_minishell *minishell, char *s_input)
{
	char	*dup;
	/*
	if (!ft_strncmp(s_input, "$?", ft_strlen(s_input)))
	{
		//Funcion que imprima el exit status
		return (//Funcion que imprima el exit status)
	}
		*/
	dup = NULL;
	if (!ft_strncmp(s_input, "$", 1))
	{
		dup = ft_strdup(s_input);
		ft_expand(&dup, minishell->env);
		return (dup);
	}
	return (ft_strdup(s_input));
}

char	*ft_group_input(t_minishell *minishell, int *i)
{
	char	*input;
	char	*tmp;

	input = ft_check_var(minishell, minishell->s_input[*i]);
	while (input && minishell->s_input[*i + 1])
	{
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
			if (input)
				ft_safe_free((void **)&input);
			input = ft_check_var(minishell, minishell->s_input[++(*i)]);
			input = ft_strjoin(tmp, input);
			free(tmp);
		}
	}
	return (input);
}

t_token_type	ft_is_operator(char *input)
{
 	if (!ft_strncmp(input, "||", 3))
		return (OR);
	else if (!ft_strncmp(input, "<<", 3))
		return (APPEND);
	else if (!ft_strncmp(input, ">>", 3))
		return (HEREDOC);
	else if (!ft_strncmp(input, ">", 2))
		return (REDIR_OUT);
	else if (!ft_strncmp(input, "<", 2))
		return (REDIR_IN);
	else if (!ft_strncmp(input, "|", 2))
		return (PIPE);
	else if (!ft_strncmp(input, "&&", 2))
		return (AND);
	else
		return (NOT_SET);
}

int	ft_is_builtin(char *input, char *next)
{
	if (!ft_strncmp(input, "echo", 5))
		return (1);
	else if (!ft_strncmp(input, "cd", 3))
		return (1);
	else if (!ft_strncmp(input, "pwd", 4))
	{
		//si pasa esto habria que meter un exit, o no se si lo gestionamos aqui o en el execve o builtin
		if (next)
			ft_printf("pwd: too many arguments\n");
		return (1);
	}
	else if (!ft_strncmp(input, "export", 7))
		return (1);
	else if (!ft_strncmp(input, "unset", 6))
		return (1);
	else if (!ft_strncmp(input, "env", 4))
		return (1);
	else if (!ft_strncmp(input, "exit", 5))
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

		}
		return (1);
	}
	else
	{
		*type = COMMAND;
		*input = ft_group_input(*minishell, i);
		return (1);
	}
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
	token = ft_create_token(input, type);
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
	(*minishell)->s_input = ft_split(input, ' ');
	if (!(*minishell)->s_input)
		return (0);
	while ((*minishell)->s_input && (*minishell)->s_input[i])
	{
		if (!ft_group_command(minishell, &i))
			return (0);
		i++;
	}
	ft_free_array((*minishell)->s_input);
	(*minishell)->s_input = NULL;
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
		i++;
	}
	name = ft_substr(str, 0, i);
	return (name);
}

char	*ft_get_value(char *str)
{
	int 	i;
	char	*value;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '=')
		{
			i++;
			break ;
		}
		i++;
	}
	if (str[i - 1] == '=' && !str[i])
		value = ft_strdup("");
	else if (!str[i])
		value = NULL;
	else
		value = ft_substr(str, i, ft_strlen(str) - i);
	return (value);
}

void	ft_change_value(char *str, t_env **node)
{
	ft_safe_free((void **)&((*node)->value));
	(*node)->value = ft_get_value(str);
	return ;
}

void	ft_free_node(t_env *node, t_env **env)
{
	t_env *tmp;
	t_env *prev;

	tmp = *env;
	prev = NULL;
	if (!node)
		return ;
	if (tmp == *env && tmp->next == NULL)
	{
		ft_safe_free((void **)&tmp->name);
		ft_safe_free((void **)&tmp->value);
		ft_safe_free((void **)&tmp);
		*env = NULL;
		return ;
	}
	while (tmp)
	{
		//ft_printf("Mi siguiente: %s, el node name %s\n", (tmp->next)->name, node->name);
		if (tmp->next && !ft_strncmp((tmp->next)->name, node->name, ft_max_strlen((tmp->next)->name, node->name)))
			prev = tmp;
		if (!ft_strncmp(tmp->name, node->name, ft_max_strlen(tmp->name, node->name)))
		{
			if (prev)
				prev->next = tmp->next;
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
		//tmp value exista, porque si es nulo y ya existe variable con ese nombre y valor, no se cambia
		if (!ft_strncmp(name_to_add, tmp->name, ft_max_strlen(name_to_add, tmp->name)))
		{
			if (!tmp->value)
			{
				ft_free_node(tmp, undefined);
				return (0);
			}
			if (value)
				ft_change_value(str, &tmp);
			ft_safe_free((void **)&name_to_add);
			return (1);
		}
		/*
		else if (!ft_strncmp(name_to_add, tmp->name, ft_strlen(name_to_add)) && !value)
		{
			//free ese token paso env tb
			//conectar los de antes y despues
			//añadir el token en el env
		}
			*/
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
char **ft_create_array_env(t_env *env)
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

t_env *ft_create_env(char	**env_array)
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
		//Esta variable: "_=/usr/bin/env" si esta en el env de bash pero no en el export, no se si deberia imprimirla o no
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

void ft_empty_export(t_minishell **shell)
{
	t_env *node;

	node = ft_create_node(ft_strdup("OLDPWD"), NULL);
	if (!node)
		return ;
	ft_connect_node(&(*shell)->undefined_var, node);
	ft_merge_lists(shell, (*shell)->env, (*shell)->undefined_var);
	return ;
}
t_env *ft_empty_env(void)
{
	t_env *env;
	t_env *node;
	char *cwd;

	env = NULL;
	//Nivel de la shell
	node = ft_create_node(ft_strdup("SHLVL"), ft_strdup("1"));
	if (!node)
		return (NULL);
	ft_connect_node(&env, node);
	//Sirve para obtener el path absoluto del directorio actual en el que se esta ejeutando el proceso
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
	(*minishell)->s_input = NULL;
	return (1);
}
//declare x hello
//declare x hi=
//declare x carlota=maja

//si hago hi=carlota se tiene que cambiar en env y export (ESTO BIEN!!!)
//si hago hello=lucas se tiene que cambiar en export y añadir a env (NO CAMBIA NADA)
//si hago carlota no se tiene que cambiar, si ya tiene valor no se cambia (LO HACE MAL, CREA OTRA VARIABLE SIN VALOR CARLOTA)
//si hago carlota= si se tiene que cambiar a hello="" y en env se cambia tambien a hello= (ESTO BIEN!!!)