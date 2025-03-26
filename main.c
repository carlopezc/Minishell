/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:58:06 by lbellmas          #+#    #+#             */
/*   Updated: 2025/03/26 15:31:42 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header/ft_minishell.h"
#include <stdio.h>
#include <unistd.h>

int	main(int argc, char **argv, char **env)
{
	t_minishell *minishell = (t_minishell *)malloc(sizeof(minishell));
	t_token		*tokens = (t_tokens *)malloc(5 * sizeof(t_token));

	minishell->tokens = tokens;
	char **c_env = (char **)malloc(sizeof(env));
	int p = 0;
	while (env[p])
	{
		c_env[p] = ft_strdup(env[p]);
		p+;
	}
	tokens[0]->str = ft_strdup("")
	ft_executor(shell);
}
