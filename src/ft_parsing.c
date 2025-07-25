/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:44:53 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/25 17:56:18 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

char	*ft_create_array(char **s_input)
{
	char	*input;
	char	*tmp;
	char	*old;
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
	while (tmp && tmp->next)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	if (tmp && tmp != *token)
	{
		if (tmp->str)
			ft_safe_free((void **)&tmp->str);
		ft_safe_free((void **)&tmp);
	}
	if (prev)
		prev->next = NULL;
	else
		*token = NULL;
	return ;
}

int	ft_check_to_quit(t_token *token)
{
	t_token	*tmp;
	int		open;
	int		close;

	open = 0;
	close = 0;
	tmp = token;
	if (tmp->type == O_BRACKET)
		tmp = tmp->next;
	while (tmp)
	{
		if (tmp->type == C_BRACKET && !open && tmp->next)
			return (0);
		else if (tmp->type == O_BRACKET)
			open++;
		else if (tmp->type == C_BRACKET && tmp->next)
			close++;
		tmp = tmp->next;
	}
	if (open == close)
		return (1);
	return (0);
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
	if (tmp->type == C_BRACKET
		&& (*token)->type == O_BRACKET && ft_check_to_quit(*token))
		ft_quit_first_last(token);
	return ;
}

int	ft_parse_spaces(char **input)
{
	int		i;
	t_quote	q;
	char	*final;
	char	*tmp;

	i = 0;
	tmp = *input;
	ft_init_quote(&q);
	final = NULL;
	while (tmp[i])
	{
		if ((tmp[i] == '\'' || tmp[i] == '\"') && (!i || tmp[i - 1] != '\\'))
			ft_check_quote(&q, tmp[i]);
		if (tmp[i] == ' ')
			final = ft_strjoin_char(final, tmp[i++]);
		while (tmp[i] && tmp[i] == ' ' && !q.flag)
			i++;
		if (tmp[i])
			final = ft_strjoin_char(final, tmp[i++]);
		if (!final)
			return (0);
	}
	free(*input);
	*input = final;
	return (1);
}

char	*ft_parsing(char **input, t_minishell **minishell)
{
	char	**s_input;
	char	*input_final;

	s_input = NULL;
	if (!input | !*input)
		return (NULL);
	if (!ft_count_quotes(*input))
		return (ft_printf("Quotes not closed\n"), NULL);
	if (!ft_count_brackets(*input))
		return (ft_printf("Brackets not closed\n"), NULL);
	s_input = ft_split_cmd(*input, ' ');
	if (!s_input || !*s_input)
		return (ft_free_array(s_input), NULL);
	input_final = ft_quit_quotes(s_input, minishell);
	if (!input_final || !*input_final)
		return (ft_free_array(s_input), NULL);
	return (ft_free_array(s_input), input_final);
}
