/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:15:05 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/26 04:29:57 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

volatile int	g_control_c;

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
			&& (tokens->next->type == PIPE
				|| tokens->next->type == AND || tokens->next->type == OR))
			return (ft_printf("Error with operators\n"), 0);
		tokens = tokens->next;
	}
	if (tokens->type == PIPE || tokens->type == AND || tokens->type == OR)
		return (ft_printf("Error with operators\n"), 0);
	ft_reorder_tokens(shell);
	return (1);
}

int	ft_just_space(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] == ' ')
		i++;
	if (!str[i] || str[i] == '\n')
		return (1);
	return (0);
}

int	ft_check_main_loop(t_minishell **minishell, char *input)
{
	if (*input && !ft_just_space(input))
	{
		add_history(input);
		if (!ft_process_input(minishell, input))
			return (ft_safe_free((void **)&input),
				ft_printf("Error processing input \n"), 2);
		if (!ft_add_bracket_token(&((*minishell)->tokens)))
			return (ft_safe_free((void **)&input),
				ft_printf("Error in brackets tokenization \n"), 2);
		if (!ft_check_wildcard(&((*minishell)->tokens)))
			return (ft_safe_free((void **)&input),
				ft_printf("Error in wildcard\n"), 2);
		if (ft_check_otokens(*minishell))
			ft_executor(*minishell);
		ft_free_tokens(minishell);
		(*minishell)->tokens = NULL;
	}
	if (g_control_c)
	{
		(*minishell)->status = 130;
		g_control_c = 0;
	}
	return (0);
}

int	ft_main_loop(t_minishell **minishell)
{
	char	*input;
	int		check;

	while (1)
	{
		ft_manage_shell_signals();
		input = readline("minishell> ");
		if (g_control_c)
		{
			(*minishell)->status = 130;
			g_control_c = 0;
		}
		if (!input)
		{
			ft_free_minishell(minishell);
			rl_clear_history();
			exit(0);
		}
		check = ft_check_main_loop(minishell, input);
		if (check != 0)
			return (check);
		ft_safe_free((void **)&input);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	*minishell;

	g_control_c = 0;
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
