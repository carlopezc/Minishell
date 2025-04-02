/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:59:32 by lbellmas          #+#    #+#             */
/*   Updated: 2025/04/02 16:08:55 by carlotalcd       ###   ########.fr       */
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
	char	**export;
	char	**s_input;
}	t_minishell;

typedef struct s_pipex
{
	int	docs[2];
	int	pipe[2][2];
	int	pid;
	char	**command;
	char	*path;
} t_pipex;

int	ft_init_minishell(t_minishell **minishell, char **env);
int	ft_process_input(t_minishell **minishell, char *input);
int	ft_executor(t_minishell *shell);
void	ft_clear_split(char **split);
void	ft_free_tokens(t_token **tokens);

	
#endif
