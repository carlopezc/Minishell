/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:59:32 by lbellmas          #+#    #+#             */
/*   Updated: 2025/03/24 15:05:30 by lbellmas         ###   ########.fr       */
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

typedef struct s_minishell
{
	t_list *tokens;
} t_minishell;

typedef struct s_token
{
	char *str;
	int	type;
} t_token;

#endif
