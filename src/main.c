/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:15:05 by carlopez          #+#    #+#             */
/*   Updated: 2025/06/01 18:56:37 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_main_loop(t_minishell **minishell)
{
	char	*input;

	while (1)
	{
		ft_manage_shell_signals();
		input = readline("minishell> ");
		if (!input)
		{
			ft_free_minishell(minishell);
			exit(0);
		}
		if (input && *input)
			add_history(input);
		if (!ft_process_input(minishell, input))
			return (ft_safe_free((void **)&input),
				ft_printf("Error processing input \n"), 2);
		if (!ft_add_bracket_token(&((*minishell)->tokens)))
			return (ft_printf("Error in brackets tokenization \n"), 2);
		if (!ft_check_wildcard(&((*minishell)->tokens)))
			return (ft_printf("Error in wildcard\n"), 2);
		ft_executor(*minishell);
		ft_safe_free((void **)&input);
		ft_free_tokens(minishell);
		(*minishell)->tokens = NULL;
	}
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	*minishell;

	minishell = NULL;
	if (argc != 1 || !argv[0])
		return (ft_printf("Wrong number of arguments (no arguments needed)\n"),
			-1);
	if (!ft_init_minishell(&minishell, env))
		return (ft_printf("Error in malloc\n"), -1);
	if (ft_main_loop(&minishell))
	{
		ft_free_tokens(&minishell);
		ft_main_loop(&minishell);
	}
	return (ft_free_minishell(&minishell), 0);
}
