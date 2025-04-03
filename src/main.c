/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:15:05 by carlopez          #+#    #+#             */
/*   Updated: 2025/04/03 20:06:58 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../header/ft_minishell.h"

char	*token_type_to_str(t_token_type type);

void	ft_free_minishell(t_minishell **minishell)
{
	if (!minishell || !*minishell)
		return ;
	if ((*minishell)->tokens)
		ft_free_tokens(minishell);
	/*
	if ((*minishell)->env)
		ft_free_array((*minishell)->env);
	if ((*minishell)->env_temporal)
		ft_free_array((*minishell)->env_temporal);
	if ((*minishell)->export)
		ft_free_array((*minishell)->export);
		*/
	if ((*minishell)->s_input)
	{
		ft_free_array((*minishell)->s_input);
		(*minishell)->s_input = NULL;
	}
	free(*minishell);
	*minishell = NULL;
	return ;
}

//solo para comprobar
void	ft_print_tokens(t_token	*token)
{
	while (token)
	{
		ft_printf("El input es %s\n", (token->str));
		ft_printf("El token type es: %s\n", token_type_to_str(token->type));
		token = token->next;
	}
	return ;
}

int	main(int argc, char **argv, char **env)
{
	char	*input;
	t_minishell	*minishell;

	(void)argc;
	(void)argv;
	if (argc != 1 || !argv[0])
		return (ft_printf("Wrong number of arguments (no argunments needed)\n"), -1);
	minishell = NULL;
	input = readline("minishell> ");
	if (!input)
		return (ft_printf("Error in function readline\n"), -1);
	if (!ft_init_minishell(&minishell, env))
		return (free(input), ft_printf("Error in malloc\n"), -1);
	while (input && *input)
	{
		add_history(input);
		if (!ft_process_input(&minishell, input))
			return (free(input), ft_free_minishell(&minishell), ft_printf("Error in process input \n"), -1);
		//ft_print_tokens(minishell->tokens);
		//ft_executor(minishell);
		free(input);
		ft_free_tokens(&minishell);
		input = readline("minishell> ");
	}
	if (input)
		free(input);
	return (ft_free_minishell(&minishell), 0);
}
