/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:15:05 by carlopez          #+#    #+#             */
/*   Updated: 2025/04/07 17:26:52 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../header/ft_minishell.h"

char	*token_type_to_str(t_token_type type);

void	ft_free_minishell(t_minishell **minishell)
{
	if (!minishell || !*minishell)
		return ;
	if ((*minishell)->tokens)
	{
		ft_free_tokens(minishell);
		(*minishell)->tokens = NULL;
	}
	if ((*minishell)->env)
	{
		ft_free_array((*minishell)->env);
		(*minishell)->env = NULL;
	}
	if ((*minishell)->env_temporal)
	{
		ft_free_array((*minishell)->env_temporal);
		(*minishell)->env_temporal = NULL;
	}
	if ((*minishell)->export)
	{
		ft_free_array((*minishell)->export);
		(*minishell)->export = NULL;
	}
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
		ft_printf("El input es %s\n", token->str);
		ft_printf("El token type es: %s\n", token_type_to_str(token->type));
		token = token->next;
	}
	return ;
}

int		ft_main_loop(t_minishell **minishell)
{
	char	*input;

	//para parar el bucle señales o exit
	while (1)
	{
		input = readline("minishell> ");
			//input puede ser nulo si recibe el EOF (CTRL + D)
		if (!input)
			return (ft_printf("exit\n"), 0);
		if (*input)
			add_history(input);
		if (!ft_process_input(minishell, input))
		{
			if (input)
				free(input);
			return (ft_printf("Error in process input \n"), -1);
		}
		//ft_print_tokens((*minishell)->tokens);
		//ft_executor(*minishell);
		if (input)
			free(input);
		input = NULL;
		ft_print_tokens((*minishell)->tokens);
		ft_free_tokens(minishell);
		(*minishell)->tokens = NULL;
	}
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	*minishell;

	(void)argc;
	(void)argv;
	minishell = NULL;
	if (argc != 1 || !argv[0])
		return (ft_printf("Wrong number of arguments (no arguments needed)\n"), -1);
	if (!ft_init_minishell(&minishell, env))
		return (ft_printf("Error in malloc\n"), -1);
	if (!ft_main_loop(&minishell))
		return (ft_free_minishell(&minishell), -1);
	return (ft_free_minishell(&minishell), 0);
}
