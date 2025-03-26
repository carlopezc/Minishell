/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:59:32 by lbellmas          #+#    #+#             */
/*   Updated: 2025/03/26 15:35:20 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "../printf/header/ft_printf.h"

typedef struct s_minishell
{
	char	**env;
	s_token *tokens;
} t_minishell;

typedef struct s_token
{
	char *str;
	int	type;
} t_token;

typedef struct s_pipex
{
	int	docs[2];
	int	pipe[2][2];
	int	pid;
	char	**command;
	char	*path;
} t_pipex;

#endif
