/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:44:53 by carlopez          #+#    #+#             */
/*   Updated: 2025/06/01 01:49:51 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

char	*ft_create_array(char **s_input)
{
	char	*input;
	char	*tmp;
	int		i;

	i = 0;
	input = NULL;
	tmp = NULL;
	if (!s_input || !*s_input)
		return (NULL);
	input = ft_strdup(s_input[i]);
	i++;
	while (s_input[i])
	{
		if (*s_input[i])
		{
			tmp = ft_strjoin(" ", s_input[i]);
			input = ft_strjoin(input, tmp);
			free(tmp);
			i++;
		}
		else
			i++;
	}
	return (input);
}

void	ft_quit_first_last(t_token **token)
{
	t_token	*tmp;
	t_token	*next;
	t_token	*prev;

	next = *token;
	*token = next->next;
	free(next->str);
	free(next);
	tmp = *token;
	while (tmp && tmp->next)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	free(tmp->str);
	free(tmp);
	prev->next = NULL;
	return ;
}

void	ft_last(t_token **token)
{
	t_token	*tmp;
	int		o_bracket;
	int		c_bracket;

	tmp = *token;
	o_bracket = 0;
	c_bracket = 0;
	if (tmp->type != O_BRACKET)
		return ;
	tmp = tmp->next;
	while (tmp && tmp->next)
	{
		if (tmp->type == O_BRACKET)
			o_bracket++;
		else if (tmp->type == C_BRACKET)
			c_bracket++;
		tmp = tmp->next;
	}
	if (o_bracket != c_bracket)
		return ;
	if (tmp->type == C_BRACKET)
		ft_quit_first_last(token);
	return ;
}

int	ft_add_bracket_token2(t_token **tmp, t_token *prev, int *i, t_token **token)
{
	char	*str;
	int		start;
	t_token	*new_token;

	str = (*tmp)->str;
	start = 0;
	if (str[*i] == '(' && (!str[*i - 1] || str[*i - 1] != '\\'))
	{
		start = *i;
		new_token = ft_create_token("(", O_BRACKET);
		if (!new_token)
			return (0);
		ft_connect_token(token, new_token, prev);
		(*tmp)->str = ft_substr(str, start + 1,
				ft_strlen(str) - (start + 1));
		str = (*tmp)->str;
		(*i)--;
		return (2);
	}
	return (1);
}

int	ft_add_bracket_token3(t_token **tmp, int *i, t_token **token)
{
	char	*str;
	t_token	*new_token;
	char	*cpy;

	str = (*tmp)->str;
	cpy = NULL;
	if (str[*i] == ')' && (!str[(*i) - 1] || str[(*i) - 1] != '\\'))
	{
		cpy = ft_strdup(str);
		(*tmp)->str = ft_substr(str, 0, *i);
		while (tmp && cpy[*i]
			&& cpy[*i] == ')' && (!cpy[(*i) - 1] || cpy[(*i)] != '\\'))
		{
			new_token = ft_create_token(")", C_BRACKET);
			if (!new_token)
				return (0);
			ft_connect_token(token, new_token, *tmp);
			(*tmp) = (*tmp)->next;
			(*i)++;
		}
		if (!cpy[*i])
			return (2);
	}
	return (1);
}

int	ft_add_brackets_loop(t_token *prev, t_token **token, t_token **tmp)
{
	int		i;
	int		res;
	char	*str;

	i = 0;
	str = (*tmp)->str;
	while (str[i])
	{
		res = ft_add_bracket_token2(tmp, prev, &i, token);
		if (!res)
			return (0);
		if (res == 1)
		{
			res = ft_add_bracket_token3(tmp, &i, token);
			if (res == 0)
				return (0);
			if (res == 2)
				break ;
		}
		i++;
	}
	return (1);
}

int	ft_add_bracket_token(t_token **token)
{
	t_token	*tmp;
	t_token	*prev;

	if (!ft_manage_brackets(*token))
		return (0);
	tmp = *token;
	if (!tmp)
		return (1);
	prev = NULL;
	while (tmp)
	{
		if (!ft_add_brackets_loop(prev, token, &tmp))
			return (0);
		prev = tmp;
		tmp = tmp->next;
	}
	ft_last(token);
	return (1);
}

int	ft_parsing(char **input, t_minishell **minishell)
{
	char	*src;
	char	**s_input;

	if (!*input | !(*input)[0])
		return (1);
	src = *input;
	if (!ft_count_quotes(src))
		return (ft_printf("Quotes not closed\n"), 0);
	if (!ft_count_brackets(src))
		return (ft_printf("Brackets not closed\n"), 0);
	s_input = ft_split_cmd(*input, ' ');
	if (!s_input || !*s_input)
		return (0);
	*input = ft_quit_quotes(s_input, minishell);
	if (!*input)
		return (0);
	return (1);
}
