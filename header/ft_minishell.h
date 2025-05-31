/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:59:23 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/31 23:50:28 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MINISHELL_H
# define FT_MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "../printf/header/ft_printf.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <dirent.h>

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
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_token
{
	char			*str;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_minishell
{
	int		status;
	t_token	*tokens;
	t_env	*env;
	t_env	*env_tmp;
	t_env	*export;
	t_env	*undefined_var;
}	t_minishell;

typedef struct s_quote
{
	char	type;
	int		flag;
}	t_quote;

typedef struct s_pipex
{
	int		brackets_count;
	int		*docs_in;
	int		*docs_out;
	int		pipe[2][2];
	int		heredoc;
	int		pid;
	int		childs;
	char	**command;
	char	*path;
}	t_pipex;

char			*token_type_to_str(t_token_type type);
int				ft_init_minishell(t_minishell **minishell, char **env);
int				ft_process_input(t_minishell **minishell, char *input);
int				ft_executor(t_minishell *shell);
void			ft_clear_split(char **split);
t_token_type	ft_is_operator(char **value, char *input, int *i);
void			ft_free_tokens(t_minishell **minishell);
int				ft_check_operator(char *input);
void			ft_add_to_env(t_minishell **minishell, char *str, int flag);
void			ft_free_array(char **arr);
void			ft_free_minishell(t_minishell **minishell);
t_env			*ft_strdup_env(t_env *env);
void			ft_safe_free(void **p);
char			**ft_create_export(char **export);
int				ft_arraylen(char **arr);
char			*ft_get_name(char *str);
char			*ft_get_value(char *str);
void			ft_manage_child_signals(void);
void			ft_print_env(t_env *env);
char			**ft_create_array_env(t_env *env);
t_env			*ft_create_env(char	**env_array);
void			ft_print_tokens(t_token	*token);
int				ft_check_duplicated(char *str, t_env **env, t_env **undefined);
t_env			*ft_create_node(char *name, char *value);
void			ft_connect_node(t_env **env, t_env *node);
char			**ft_split_cmd(char const *s, char c);
int				ft_listlen(t_env *env);
void			ft_free_todo(int p, char **word);
void			ft_check_quote(t_quote *quote, char c);
void			ft_free_env(t_env **env);
void			ft_print_export(t_env *export);
char			*ft_expand(char *input, int *i, t_env *env);
char			*ft_strjoin_char(char *str, char c);
char			*ft_choose_larger(char *str, char *str2);
void			ft_merge_lists(t_minishell **shell,
					t_env *first, t_env *second);
size_t			ft_max_strlen(char *s1, char *s2);
void			ft_free_node(t_env *node, t_env **list);
void			ft_print_array(char **arr);
int				ft_parsing(char **input, t_minishell **minishell);
int				ft_manage_brackets(t_token *tokens);
char			*ft_check_var(t_minishell *minishell, char *input, int *i);
t_token			*ft_create_token(char *str, t_token_type type);
void			ft_connect_token(t_token **tokens, t_token *new, t_token *prev);
int				ft_add_bracket_token(t_token **token);
int				ft_check_wildcard(t_token **tokens);
char			*ft_create_array(char **s_input);
void			ft_free_todo(int p, char **word);

//ft_token_utils.c
void			ft_add_node_back(t_token **lst, t_token *new);

//ft_count.c
int				ft_count_brackets(char *str);
int				ft_count_quotes(char *str);
int				ft_count_files(DIR *dir);

//ft_aux_check.c
int				ft_last_check(t_token *tokens);
int				ft_check_brackets(t_token *token);
int				ft_is_builtin(char *input);

//ft_init.c
void			ft_init_quote(t_quote *quote);

//ft_brackets.c
int				ft_quit_brckt_dup(t_token *tmp, char c);
int				ft_quit_brackets(t_token *token, int *open, int *close);

//ft_quotes.c
void			ft_unquote(char **input, int flag);
char			*ft_quit_quotes(char **s_input, t_minishell **minishell);

//ft_get_elements.c
char			*ft_get_next(char *input, int *i);
int				ft_get_size(char *input, char c);

//ft_env.c
t_env			*ft_empty_env(void);

//ft_export.c
void			ft_empty_export(t_minishell **shell);
void			ft_change_value(char *str, t_env **node);

//ft_signals.c
void			ft_manage_shell_signals(void);

//ft_testing_tools.c
char			*token_to_str(t_token_type type);

//ft_variable.c
int				ft_variable(char **input, t_minishell **minishell);

//ft_file_management.c
char			**ft_get_elements(void);
int				ft_delete_item(char ***elements, char *delete);

//ft_asterisk.c
int				ft_check_asterisk(char *input, char ***elements);
int				ft_find_asterisk(char *input);
int				ft_parse_asterisk(char **input);
char			**ft_split_asterisk(char *input, char c);

void	ft_init_int(int (*i)[3]);

#endif
