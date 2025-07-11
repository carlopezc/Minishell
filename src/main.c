/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:15:05 by carlopez          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/06/30 20:38:29 by carlotalcd       ###   ########.fr       */
=======
/*   Updated: 2025/07/03 15:36:46 by lbellmas         ###   ########.fr       */
>>>>>>> fb62861378999c7b00409ed7e682ba8d49109295
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_check_otokens(t_minishell *shell)
{
	t_token	*tokens;

	tokens = shell->tokens;
	if (!tokens)
		return (0);
	if (tokens->type == PIPE || tokens->type == AND || tokens->type == OR)
		return (ft_printf("Error with operators\n"), 0);
	while (tokens && tokens->next)
	{
		if ((tokens->type == PIPE || tokens->type == AND || tokens->type == OR)
				&& (tokens->next->type == PIPE || tokens->next->type == AND || tokens->next->type == OR))
			return (ft_printf("Error with operators\n"), 0);
		tokens = tokens->next;
	}
	if (tokens->type == PIPE || tokens->type == AND || tokens->type == OR)
		return (ft_printf("Error with operators\n"), 0);
	return (1);
}

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
<<<<<<< HEAD
		ft_executor(*minishell);
=======
		ft_printf("Tokens finales: \n");
		ft_print_tokens((*minishell)->tokens);
		if (ft_check_otokens(*minishell))
			ft_executor(*minishell);
>>>>>>> fb62861378999c7b00409ed7e682ba8d49109295
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
	while (ft_main_loop(&minishell))
		ft_free_tokens(&minishell);
	return (ft_free_minishell(&minishell), 0);
}
