/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:26:52 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/26 01:17:43 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void    ft_special_swap(t_token *token, t_minishell *shell)
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
			&& ((list->next->type == COMMAND) || (list->next->type == EXEC)
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

void	ft_reorder_tokens(t_minishell *shell)
{
	t_token	*tmp;
	int	i;
	int	flag;

	i = 1;
	flag = 0;
	tmp = shell->tokens; 
	while (tmp)
	{
		flag = 0;
		if (tmp->type == COMMAND)
			i = 0;
		else if (tmp->type == PIPE || tmp->type == AND
			|| tmp->type == OR || tmp->type == C_BRACKET)
			i = 1;
		else if ((tmp->type == REDIR_IN || tmp->type == HEREDOC
			|| tmp->type == REDIR_OUT || tmp->type == APPEND) && i)
		{
			if (ft_reorder(tmp, shell))
				tmp = shell->tokens;
			else
				tmp = tmp->next;
			flag = 1;
		}
		if (!flag)
			tmp = tmp->next;
	}
}

void	ft_add_node_back(t_token **lst, t_token *new)
{
	t_token	*temp;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
	return ;
}

t_token	*ft_create_token(char *str, t_token_type type)
{
	t_token	*token;

	if (!str)
		return (NULL);
	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->str = ft_strdup(str);
	token->type = type;
	token->next = NULL;
	return (token);
}

void	ft_connect_token(t_token **tokens, t_token *new, t_token *prev)
{
	t_token	*tmp;
	t_token	*save;

	save = NULL;
	tmp = *tokens;
	if (!tokens)
		return ;
	if (!prev)
	{
		new->next = *tokens;
		*tokens = new;
		return ;
	}
	while (tmp)
	{
		if (tmp == prev)
		{
			save = tmp->next;
			tmp->next = new;
			new->next = save;
			return ;
		}
		tmp = tmp->next;
	}
	return ;
}
