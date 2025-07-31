/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:05:18 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/31 18:37:39 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_set_to_quit(int *open, int *close, int *to_quit)
{
	*to_quit = *open;
	if (*open > *close)
		*to_quit = *close;
	*open -= *to_quit;
	*close -= *to_quit;
	return ;
}

void	ft_check_in_word(int *in_word, char c)
{
	if (*in_word && c == ' ')
		*in_word = 0;
	else if (!*in_word && c != ' ')
		*in_word = 1;
	return ;
}

void	ft_check_quote(t_quote *quote, char c)
{
	if (c == '\'' || c == '\"')
	{
		if (quote->flag == 0)
		{
			quote->flag = 1;
			quote->type = c;
		}
		else if (quote->type != c)
			return ;
		else if (quote->type == c)
		{
			quote->flag = 0;
			quote->type = 0;
		}
	}
	return ;
}

int	ft_check_heredoc(t_token *save, t_pipex *pipex, t_minishell *shell)
{
	t_token	*tmp;

	tmp = save;
	while (tmp && tmp->type != PIPE && tmp->type != AND && tmp->type != OR)
	{
		if (tmp->type == HEREDOC)
		{
			tmp = ft_heredoc(tmp, pipex, shell);
			if (!tmp)
				return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

int	ft_check_operator(char *input)
{
	if (!input || input[0] == '\0')
		return (0);
	else if (*input == '(' || *input == ')')
		input = input + 1;
	if (!ft_strncmp(input, "||", 2) || !ft_strncmp(input, "|", 1))
		return (1);
	else if (!ft_strncmp(input, ">>", 2) || !ft_strncmp(input, ">", 1))
		return (1);
	else if (!ft_strncmp(input, "<<", 2) || !ft_strncmp(input, "<", 1))
		return (1);
	else if (!ft_strncmp(input, "&&", 2))
		return (1);
	else
		return (0);
}
