/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:15:05 by carlopez          #+#    #+#             */
/*   Updated: 2025/03/27 15:39:01 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../header/ft_minishell.h"

int	main(int argc, char **argv, char **env)
{
	char	*input;
	t_minishell	*minishell;

	(void)argc;
	(void)argv;
	minishell = NULL;
	input = readline("minishell> ");
	if (!input)
		return (ft_printf("Error in function readline\n"), -1);
	if (!ft_init_minishell(&minishell, env))
		return (free(input), ft_printf("Error in malloc\n"), -1);
	while (input && *input)
	{
		if (!ft_process_input(&minishell, input))
			return (free(input), free(minishell), ft_printf("Error in process input \n"), -1);		
		free(input);
		ft_executor(minishell);
		input = readline("minishell> ");
	}
	if (input)
		free(input);
	return (free(minishell), 0);
		
}
