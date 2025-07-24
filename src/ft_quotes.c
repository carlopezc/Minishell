/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:15:42 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/24 19:50:39 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_is_quote(char c)
{
	if (c == '\'' || c == '\"')
		return (1);
	return (0);
}

char	*ft_simp_unquote(char *input, int *i, t_quote q)
{
	char	*unquoted;

	unquoted = NULL;
	if (input[*i] != '\'')
		return (NULL);
	if (!q.flag || (q.type == '\''))
		(*i)++;
	while (input[*i] && input[*i] != '\'')
	{
		if (input[*i] == '*')
			unquoted = ft_strjoin_char(unquoted, '\\');
		unquoted = ft_strjoin_char(unquoted, input[(*i)++]);
	}
	if (!q.flag || (q.type == '\''))
		(*i)++;
	return (unquoted);
}

void	ft_aux_unquote(char *input, int *i, char **final, t_quote q)
{
	if (input[*i] == '*' && q.flag)
	{
		*final = ft_strjoin_char(*final, '\\');
		*final = ft_strjoin_char(*final, '*');
		(*i)++;
	}
	else if (input[*i] == '\\' && (!(*i) || input[(*i) - 1] != '\\'))
		(*i)++;
	else
		*final = ft_strjoin_char(*final, input[(*i)++]);
}

void	ft_unquote_loop(char **input, int *i, char **final, t_quote *q)
{
	char	*simp;
	int		in_word;

	simp = NULL;
	in_word = 1;
	if ((*input)[*i] == '\'' && (!(*i) || (*input)[*i - 1] != '\\'))
	{
		simp = ft_simp_unquote(*input, i, *q);
		*final = ft_strjoin(*final, simp);
		ft_safe_free((void **)&simp);
		return ;
	}
	ft_check_in_word(&in_word, (*input)[*i]);
	if (((*input)[*i] == '\'' || (*input)[*i] == '\"')
		&& (!(*i) || (*input)[*i - 1] != '\\'))
	{
		ft_check_quote(q, (*input)[*i]);
		if (in_word)
			(*i)++;
		else
			*final = ft_strjoin_char(*final, (*input)[(*i)++]);
	}
	else
		ft_aux_unquote(*input, i, final, *q);
}

void	ft_unquote(char **input, int flag)
{
	t_quote	q;
	char	*final;
	int		i;

	if (flag || !input || !*input || !**input)
		return ;
	ft_init_quote(&q);
	final = NULL;
	i = 0;
	while ((*input)[i])
		ft_unquote_loop(input, &i, &final, &q);
	ft_safe_free((void **)input);
	*input = final;
}
