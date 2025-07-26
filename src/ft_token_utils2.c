/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 02:52:03 by lbellmas          #+#    #+#             */
/*   Updated: 2025/07/26 02:53:13 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_special_swap(t_token *token, t_minishell *shell)
{
	t_token	*tmp;
	t_token	*com;
	t_token	*prev;

	tmp = token;
	while (tmp && tmp->type != PIPE && tmp->type != AND
		&& tmp->type != OR && tmp->type != C_BRACKET)
		tmp = tmp->next;
	com = shell->tokens;
	while (com->type != COMMAND && com->type != EXEC && com->type != BUILTIN)
		com = com->next;
	prev = shell->tokens;
	if (prev == token)
		shell->tokens = token->next;
	else
	{
		while (prev->next != token)
			prev = prev->next;
		prev->next = token->next;
	}
	token->next = tmp;
	while (com->next != tmp)
		com = com->next;
	com->next = token;
}

void	ft_swap_tokens(t_token *token, t_minishell *shell)
{
	t_token	*tmp;
	t_token	*next;
	t_token	*prev;

	tmp = shell->tokens;
	prev = tmp;
	while (tmp)
	{
		if (tmp == token)
		{
			prev->next = next;
			next->next = tmp;
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
		next = tmp->next;
	}
}

int	ft_check_reorder(t_token *list)
{
	while (list && list->type != PIPE && list->type != AND
		&& list->type != OR && list->type != C_BRACKET)
	{
		if (list->type == COMMAND)
			return (1);
		list = list->next;
	}
	return (0);
}

int	ft_reorder(t_token *token, t_minishell *shell)
{
	t_token	*list;

	list = shell->tokens;
	while (list)
	{
		if (token == list)
		{
			if ((list->type == HEREDOC) && (list->next
					&& ((list->next->type == COMMAND)
						|| (list->next->type == EXEC)
						|| (list->next->type == BUILTIN))))
			{
				ft_swap_tokens(token, shell);
				return (1);
			}
			else if (((list->type == REDIR_IN) || (list->type == REDIR_OUT)
					|| (list->type == APPEND)) && ft_check_reorder(list))
			{
				ft_special_swap(token, shell);
				return (1);
			}
		}
		list = list->next;
	}
	return (0);
}
