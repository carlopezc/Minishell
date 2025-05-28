/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:59:32 by lbellmas          #+#    #+#             */
/*   Updated: 2025/05/28 11:23:37 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "../printf/header/ft_printf.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <dirent.h>

typedef enum e_token_type
{
	PIPE,
	EXEC,
	COMMAND,
	BUILTIN,
	REDIR_IN,
	REDIR_OUT,
	O_BRACKET,
	C_BRACKET,
	HEREDOC,
	APPEND,
	AND,
	OR,
	NOT_SET,
	ERROR	
}	t_token_type;

typedef struct s_env
{
	char *name;
	char	*value;
	struct s_env *next;
}	t_env;

typedef struct s_token
{
	char *str;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_minishell
{
	int		status;
	t_token *tokens;
	t_env	*env;
	t_env	*env_tmp;
	t_env	*export;
	t_env 	*undefined_var;
}	t_minishell;

typedef struct s_quote
{
	char type;
	int flag;
}	t_quote;

typedef struct s_pipex
{
	int	brackets_count;
	int	*docs_in;
	int	*docs_out;
	int	pipe[2][2];
	int	heredoc;
	int	pid;
	int	childs;
	char	**command;
	char	*path;
} t_pipex;

char	*token_type_to_str(t_token_type type);
int	ft_init_minishell(t_minishell **minishell, char **env);
int	ft_process_input(t_minishell **minishell, char *input);
int	ft_executor(t_minishell *shell);
void	ft_clear_split(char **split);
t_token_type	ft_is_operator(char **value, char *input, int *i);
void	ft_free_tokens(t_minishell **minishell);
int	ft_check_operator(char *input);
void 	ft_add_to_env(t_minishell **minishell, char *str, int flag);
void	ft_free_array(char **arr);
void	ft_free_minishell(t_minishell **minishell);
t_env	*ft_strdup_env(t_env *env);
void	ft_safe_free(void **p);
char	**ft_create_export(char **export);
int	ft_arraylen(char **arr);
char	*ft_get_name(char *str);
char	*ft_get_value(char *str);
void	ft_manage_child_signals();
void	ft_print_env(t_env *env);
char **ft_create_array_env(t_env *env);
t_env *ft_create_env(char	**env_array);
void	ft_print_tokens(t_token	*token);
int	ft_check_duplicated(char *str, t_env **env, t_env **undefined);
t_env	*ft_create_node(char *name, char *value);
void	ft_connect_node(t_env **env, t_env *node);
char	**ft_split_cmd(char const *s, char c);
int	ft_listlen(t_env *env);
void	ft_free_todo(int p, char **word);
void	ft_check_quote(t_quote *quote, char c, int *i);
void	ft_free_env(t_env **env);
void	ft_print_export(t_env *export);
char	*ft_expand(char *input, int *i, t_env *env);
char	*ft_strjoin_char(char *str, char c);
char *ft_choose_larger(char *str, char *str2);
void	ft_merge_lists(t_minishell **shell, t_env *first, t_env *second);
size_t	ft_max_strlen(char *s1, char *s2);
void	ft_free_node(t_env *node, t_env **list);
void	ft_print_array(char **arr);
int	ft_parsing(char **input, t_minishell **minishell);
int	ft_manage_brackets(t_token *tokens);
char	*ft_check_var(t_minishell *minishell, char *input, int *i);
t_token	*ft_create_token(char *str, t_token_type type);
void	ft_connect_token(t_token **tokens, t_token *new, t_token *prev);
int	ft_add_bracket_token(t_token **token);
int	ft_check_wildcard(t_token **tokens);
char	*ft_create_array(char **s_input);

#endif
