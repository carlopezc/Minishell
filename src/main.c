/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:15:05 by carlopez          #+#    #+#             */
/*   Updated: 2025/04/29 14:04:13 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../header/ft_minishell.h"

void	ft_free_env(t_env **env)
{
	t_env	*tmp;
	t_env	*next;

	if (!env)
		return ;
	tmp = *env;
	while (tmp)
	{
		next = tmp->next;
		ft_safe_free((void **)&tmp->name);
		ft_safe_free((void **)&tmp->value);
		free(tmp);
		tmp = next;
	}
	*env = NULL;
	return ;
}

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
		ft_free_env(&(*minishell)->env);
	if ((*minishell)->export)
		ft_free_env(&(*minishell)->export);
	if ((*minishell)->undefined_var)
		ft_free_env(&(*minishell)->undefined_var);
	free(*minishell);
	*minishell = NULL;
	return ;
}

void	ft_print_tokens(t_token	*token)
{
	t_token	*tmp;

	tmp = token;
	while (tmp)
	{
		ft_printf("El input es %s\n", tmp->str);
		ft_printf("El token type es: %s\n", token_type_to_str(tmp->type));
		tmp = tmp->next;
	}
	return ;
}

void	ft_manage_sigint(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	return ;
}

void	ft_manage_shell_signals()
{
	signal(SIGINT, ft_manage_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	ft_manage_child_signals()
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

int	ft_main_loop(t_minishell **minishell)
{
	char	*input;

	while (1)
	{
		input = readline("minishell> ");
			//input puede ser nulo si recibe el EOF (CTRL + D), esto puede que este mal
		if (!input)
			return (ft_printf("exit\n"), 0);
		if (*input)
			add_history(input);
		if (!ft_process_input(minishell, input))
		{
			if (input)
				free(input);
			return (ft_free_minishell(minishell), ft_printf("Error in process input \n"), -1);
		}
		ft_printf("Lista de tokens\n");
		ft_print_tokens((*minishell)->tokens);
		//ft_executor(*minishell);
		if (input)
			free(input);
		input = NULL;
		/*
		ft_printf("\nLISTA DE TOKENS : \n");
		ft_print_tokens((*minishell)->tokens);
		ft_printf("\nENVIRONMENT : \n");
		ft_print_array((*minishell)->env);
		ft_printf("\nEXPORT : \n");
		ft_print_array((*minishell)->export);
		*/
		ft_free_tokens(minishell);
		(*minishell)->tokens = NULL;
		/*
		ft_free_array((*minishell)->env_temporal);
		(*minishell)->env_temporal = NULL;
		*/
	}
}

void	ft_print_array(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
		ft_printf("%s\n", arr[i++]);
	return ;
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	*minishell;

	(void)argc;
	(void)argv;
	minishell = NULL;
	if (argc != 1 || !argv[0])
		return (ft_printf("Wrong number of arguments (no arguments needed)\n"), -1);
	ft_manage_shell_signals();
	if (!ft_init_minishell(&minishell, env))
		return (ft_printf("Error in malloc\n"), -1);
	if (!ft_main_loop(&minishell))
		return (ft_free_minishell(&minishell), -1);
	return (ft_free_minishell(&minishell), 0);
}
