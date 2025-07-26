/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:59:23 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/26 05:56:28 by carlopez         ###   ########.fr       */
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
# include <sys/wait.h>
# include <fcntl.h>

extern volatile int	g_control_c;

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
void			ft_print_export(t_minishell *shell);
char			*ft_expand(char *input, int *i, t_env *env);
char			*ft_strjoin_char(char *str, char c);
char			*ft_choose_larger(char *str, char *str2);
void			ft_merge_lists(t_minishell **shell,
					t_env *first, t_env *second);
size_t			ft_max_strlen(char *s1, char *s2);
void			ft_free_node(t_env *node, t_env **list);
void			ft_print_array(char **arr);
char			*ft_parsing(char **input, t_minishell **minishell);
int				ft_manage_brackets(t_token *tokens);
char			*ft_check_var(t_minishell *minishell, char *input, int *i);
t_token			*ft_create_token(char *str, t_token_type type);
void			ft_connect_token(t_token **tokens, t_token *new, t_token *prev);
int				ft_add_bracket_token(t_token **token);
int				ft_check_wildcard(t_token **tokens);
char			*ft_create_array(char **s_input);
void			ft_free_todo(int p, char **word);
int				ft_is_quote(char c);
int				ft_check_brackets(t_token *token);
int				ft_brackets_loop(char *value, int *i, char c);
char			*ft_expand_wildcard(char *input, t_token_type type);
int				ft_cmp_except(char *str, char *str2, int n);
t_env			*ft_search_pwd(t_minishell *shell);
int				ft_parse_spaces(char **input);
int				ft_first_or_last(char *str, int i);
void			ft_quit_first_last(t_token **token);
int				ft_reorder(t_token *token, t_minishell *shell);
int				ft_get_value_join(char **value,
					char *equal, int *i, char **tmp);
char			*ft_clean_value(char *value);

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

//ft_aux_aux_check
int				ft_is_quote(char c);

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
void			ft_normal_signals(void);
void			ft_manage_here_signals(void);

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

void			ft_init_int(int (*i)[3]);
int				ft_check_next(t_token *tokens, int o_brckt);
void			ft_last(t_token **token);
int				ft_simp(int simp);
int				ft_asterisk(int asterisk);
int				ft_unquote_if(int i, int flag, char **input);
int				ft_unquote_if2(int i, char **input);
void			ft_unquote_inside2(int *in_word, int *i,
					char **input, char **unquoted);
int				ft_unquote_if4(int i, int flag, char **input);
int				ft_unquote_if3(int i, char **input);
int				ft_unquote_inside5(int *i, char **unquoted,
					char **input, int *simp);
int				ft_unquote_inside4(int *in_word,
					int *simp, int asterisk);
void			ft_unquote_inside3(int *in_word, int *i,
					char **input, char **unquoted);
void			ft_unquote_inside(int *in_word, int *i,
					char **input, char **unquoted);
void			ft_unquote_first(int (*i)[4], char **input,
					char **unquoted, int flag);
void			ft_asterisk_init(int *i, int *in_word,
					int *simp, int *asterisk);
//ft_executor
void			ft_terminator(t_pipex *pipex);
t_token			*ft_analisis_comands(t_pipex *pipex,
					t_minishell *shell, t_token **save);
t_token			*ft_analisis_redir(t_token *save, t_pipex *pipex, t_token *tmp);
t_token			*ft_exectime(t_pipex *pipex, t_minishell *shell,
					t_token *tmp, t_token *save);
void			ft_arrange_fd(t_pipex *pipex);
t_pipex			*ft_init_pipex(void);
void			ft_free_pipex(t_pipex **pipex);
char			*ft_errase_route(char *route);
void			ft_cd_add(t_minishell *shell, char *temp);
int				ft_check_cd(char *file, char *pwd, t_minishell *shell);
void			ft_old_pwd(t_env **env, t_env *pwd);
char			*ft_correct_cd(char *path);
void			ft_add_node(t_env **list, t_env *prev, t_env *node);
void			ft_errase_pwd(t_minishell *shell);
void			ft_cd_home(t_minishell *shell);
void			ft_swap_list(t_env *a, t_env *b);
void			ft_sort_list(t_env *head);
void			ft_cd(t_minishell *shell, char *cmd);
void			ft_pwd(t_minishell *shell);
void			ft_export(t_minishell *shell, char *cmd);
void			ft_env(t_minishell *shell, char *cmd);
int				ft_check_docs(int *docs);
void			ft_docs_in(t_pipex *pipex);
void			ft_docs_out(t_pipex *pipex);
void			ft_exec_build(t_minishell *shell, char *cmd);
void			ft_pre_exec_command(t_pipex *pipex,
					t_token *cmd, t_minishell *shell);
int				ft_path(t_env **env, t_pipex **pipex, char *cmd);
t_token			*ft_redir(t_token *save, t_token_type type, t_pipex *pipex);
void			ft_exec(t_minishell *shell, t_pipex *pipex, t_token *save);
void			ft_terminator(t_pipex *pipex);
t_token			*ft_killchilds(t_pipex *pipex,
					t_minishell *shell, t_token *save);
t_token			*ft_and(t_pipex *pipex, t_minishell *shell, t_token *save);
t_token			*ft_or(t_pipex *pipex, t_minishell *shell, t_token *save);
int				ft_check_name(char *var);
char			*ft_quit_quotes(char **s_input, t_minishell **minishell);
char			*token_to_str(t_token_type type);
void			ft_cd_route(t_minishell *shell, char *cmd);
int				ft_word_count(char const *w, char c);
void			ft_docs_in_child(t_pipex *pipex);
void			ft_docs_out_true(t_pipex *pipex);
void			ft_heredoc_lonely(t_pipex *pipex);
void			ft_echo(char *cmd);
size_t			ft_max_strlen(char *s1, char *s2);
int				ft_handle_quotes_in_word(char *str, int i, char **final);
int				ft_handle_normal_chars(char *str,
					int i, t_quote *q, char **final);
void			ft_handle_quotes_and_skip(char *str,
					int *i, t_quote *q, int *s_flag);
int				ft_handle_backslash_quotes(char *str,
					int i, t_quote *q, char **final);
int				ft_find_asterisk_in_word(const char *str, int index);
int				ft_get_pattern(char *str, int *i,
					char **str_final, t_token **tmp);
int				ft_handle_escaped_asterisk(char *str,
					int *i, int *flag, char **final);
int				ft_process_asterisk_pattern(char *str,
					int *i, char **str_final, t_token **tmp);
int				ft_is_real_asterisk(char *str, int i, t_quote *q);
void			ft_check_exit(t_pipex *pipex, t_minishell *shell);
void			ft_exit(t_pipex *pipex, t_minishell *shell, t_token *save);
void			ft_check_exit(t_pipex *pipex, t_minishell *shell);
t_token_type	ft_is_operator4(char **value, char *input, int *i, int flag);
t_token_type	ft_handle_redirect_append(char **value,
					char *input, int *i, int flag);
t_token_type	ft_handle_redirect_out(char **value,
					char *input, int *i, int flag);
t_token_type	ft_handle_pipe_or(char **value,
					char *input, int *i, int flag);
t_token_type	ft_handle_redirect_in(char **value,
					char *input, int *i, int flag);
int				ft_check_error(char *input, int i, int c);
t_token			*ft_and(t_pipex *pipex, t_minishell *shell, t_token *save);
t_token			*ft_or(t_pipex *pipex, t_minishell *shell, t_token *save);
void			ft_check_in_word(int *in_word, char c);
void			ft_set_to_quit(int *open, int *close, int *to_quit);
int				ft_add_while(char *value, int *i, char c, char **new_value);
int				ft_first_or_last(char *str, int i);
void			ft_skip_spaces(char *input, int *i);
int				ft_if_no_next(t_env **tmp, t_env **list);
int				ft_check_heredoc(t_token *save, t_pipex *pipex);
// ft_heredoc.
t_token			*ft_heredoc(t_token *save, t_pipex *pipex);
void			ft_skip_spaces(char *input, int *i);
void			ft_reorder_tokens(t_minishell *shell);
void			ft_finish_build(int num, t_minishell *shell);
#endif
