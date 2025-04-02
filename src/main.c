/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:15:05 by carlopez          #+#    #+#             */
/*   Updated: 2025/04/02 15:52:05 by carlotalcd       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../header/ft_minishell.h"

char	*token_type_to_str(t_token_type type);

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
		t_token *token;
		token = minishell->tokens;
		while (token)
		{
			ft_printf("El input es %s\n", (token->str));
			ft_printf("El token type es: %s\n", token_type_to_str(token->type));
			token = token->next;
		}
		//ft_executor(minishell);
		free(input);
		//Solo para ver la tokenizacion
		
		input = readline("minishell> ");
	}
	return (0);
}
