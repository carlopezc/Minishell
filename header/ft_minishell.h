/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:59:32 by lbellmas          #+#    #+#             */
/*   Updated: 2025/04/10 21:41:44 by carlotalcd       ###   ########.fr       */
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

typedef enum e_token_type
{
	PIPE,
	EXEC,
	COMMAND,
	BUILTIN,
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND,
	AND,
	OR,
	NOT_SET	
}	t_token_type;

typedef struct s_token
{
	char *str;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_minishell
{
	t_token *tokens;
	char	**env;
	char	**env_temporal;
	char	**export;
	char	**s_input;
}	t_minishell;

typedef struct s_pipex
{
	int	*docs_in;
	int	*docs_out;
	int	pipe[2][2];
	int	pid;
	int	childs;
	char	**command;
	char	*path;
} t_pipex;

int	ft_init_minishell(t_minishell **minishell, char **env);
int	ft_process_input(t_minishell **minishell, char *input);
int	ft_executor(t_minishell *shell);
void	ft_clear_split(char **split);
void	ft_free_tokens(t_minishell **minishell);
void 	ft_add_to_env(t_minishell **minishell, char *str, int flag);
void	ft_free_array(char **arr);
void	ft_free_minishell(t_minishell **minishell);
char	**ft_strdup_env(char **env);
void	ft_safe_free(void **p);
char	**ft_create_export(char **export);
int	ft_arraylen(char **arr);
char	*ft_get_name(char *str);
void	ft_manage_child_signals();
	
#endif
