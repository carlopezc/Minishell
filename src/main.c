/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:15:05 by carlopez          #+#    #+#             */
/*   Updated: 2025/04/02 14:57:15 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../header/ft_minishell.h"

void	ft_free_tokens(t_minishell *minishell)
{
	int	p;

	if (!minishell || !minishell->tokens)
		return ;
	p = 0;
	while ((p * sizeof(t_token) < sizeof(minishell->tokens)))
	{
		free(minishell->tokens[p++].str);
	}
	free(minishell->tokens);
}

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
		ft_printf("%s\n", minishell->tokens[0].str);
		//ft_free_tokens(minishell);
		input = readline("minishell> ");
	}
	if (input)
		free(input);
	return (free(minishell), 0);
		
}
