/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:44:53 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/15 13:49:44 by carlopez         ###   ########.fr       */
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
	ft_printf("S_input es : \n");
	ft_print_array(s_input);
	if (!s_input || !*s_input)
		return (0);
	*input = ft_quit_quotes(s_input, minishell);
	if (!*input)
		return (ft_free_array(s_input), 0);
	return (ft_free_array(s_input), 1);
}
