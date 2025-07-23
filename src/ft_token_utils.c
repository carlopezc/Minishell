/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:26:52 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/24 00:20:35 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

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
	printf("🔸Token creado: [%s] tipo: %s\n", token->str, token_to_str(token->type));
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
