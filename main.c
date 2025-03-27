/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:25:20 by carlopez          #+#    #+#             */
/*   Updated: 2025/03/25 18:54:08 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "header/ft_minishell.h"

void	ft_free_array(char **arr)
{
	int	i;

	if (!arr || !*arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	ft_free_node(t_list *token)
{
	if (!token)
		return ;
	if (token->content && ((t_token *)token->content)->str)
		free(((t_token *)token->content)->str);
	if (token->content)
		free(token->content);
	return ;
}

void	ft_free_list(t_list **tokens)
{
	t_list	*rem;
	t_list	*mov;

	if (!tokens)
		return ;
	rem = *tokens;
	while (rem)
	{
		mov = rem->next;
		ft_free_node(rem);
		rem = mov;
	}
	*tokens = NULL;
}

void	ft_free_struct(t_minishell *minishell)
{
	if (!minishell)
		return ;
	ft_free_list(&(minishell->tokens));
	free(minishell);
	return ;
}

//HAY LEAKS POR TODOS LADOS
int	main(int argc, char **env)
{
	char    *input;
	char	**s_input;
	t_minishell	*minishell;

	(void)argc;
	minishell = ft_init_struct();
	if (!minishell)
		return (-1);
	minishell->env = env;
	input = readline("minishell> ");
	if (!input || !*input)
		return (ft_free_struct(minishell), -1);
	s_input = ft_split(input, ' ');
	//faltan frees de muchas cosas!!
	if (!s_input)
	    return (ft_free_struct(minishell), free(input), -1);
	while (input && *input)
	{
		add_history(input);
		s_input = ft_split(input, ' ');
		ft_tokenization(s_input, minishell);
		free(input);
		if (s_input)
			ft_free_array(s_input);
		input = readline("minishell> ");
	}
	//tokenization(input);
	if (input)
		free(input);
	if (s_input && *s_input)
		ft_free_array(s_input);
	return (ft_free_struct(minishell), 0);
}
