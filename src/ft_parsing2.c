/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 19:22:41 by lbellmas          #+#    #+#             */
/*   Updated: 2025/07/25 19:23:32 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

static void	ft_init_create(int *i, char **tmp, char **input)
{
	*i = 0;
	*tmp = NULL;
	*input = NULL;
}

char	*ft_create_array(char **s_input)
{
	char	*input;
	char	*tmp;
	char	*old;
	int		i;

	ft_init_create(&i, &tmp, &input);
	if (!s_input || !*s_input)
		return (NULL);
	input = ft_strdup(s_input[i]);
	i++;
	while (s_input[i])
	{
		if (*s_input[i])
		{
			tmp = ft_strjoin(" ", s_input[i]);
			old = input;
			input = ft_strjoin(input, tmp);
			ft_safe_free((void **)&tmp);
			ft_safe_free((void **)&old);
			i++;
		}
		else
			i++;
	}
	return (input);
}

static t_token	*ft_quit_first2(t_token *tmp, t_token **prev, t_token **token)
{
	while (tmp && tmp->next)
	{
		*prev = tmp;
		tmp = tmp->next;
	}
	if (tmp && tmp != *token)
	{
		if (tmp->str)
			ft_safe_free((void **)&tmp->str);
		ft_safe_free((void **)&tmp);
	}
	return (tmp);
}

void	ft_quit_first_last(t_token **token)
{
	t_token	*tmp;
	t_token	*prev;

	if (!token || !*token)
		return ;
	tmp = *token;
	*token = tmp->next;
	prev = NULL;
	if (tmp->str)
		ft_safe_free((void **)&tmp->str);
	ft_safe_free((void **)&tmp);
	if (!*token)
		return ;
	tmp = *token;
	prev = NULL;
	tmp = ft_quit_first2(tmp, &prev, token);
	if (prev)
		prev->next = NULL;
	else
		*token = NULL;
}
