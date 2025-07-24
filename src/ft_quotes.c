/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:15:42 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/24 05:48:16 by carlopez         ###   ########.fr       */
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

void	ft_check_in_word(int *in_word, char c)
{
	if (*in_word && c == ' ')
		*in_word = 0;
	else if (!*in_word && c != ' ')
		*in_word = 1;
	return ;
}

void	ft_unquote(char **input, int flag)
{
	int		i;
	t_quote	q;
	int		in_word;
	char	*final;
	char	*simp;

	i = 0;
	ft_init_quote(&q);
	final = NULL;
	in_word = 1;
	simp = NULL;
	if (flag || !input || !*input || !**input)
		return ;
	while (ft_strlen(*input) >= (size_t)i && (*input)[i])
	{
		if ((*input)[i] == '\'' && (!i || (*input)[i - 1] != '\\'))
		{
			simp = ft_simp_unquote(*input, &i, q);
			final = ft_strjoin(final, simp);
			ft_safe_free((void **)&simp);
		}
		ft_check_in_word(&in_word, (*input)[i]);
		if (((*input)[i] == '\'' || (*input)[i] == '\"')
				&& (!i || (*input)[i - 1] != '\\'))
		{
			ft_check_quote(&q, (*input)[i]);
			if (in_word)
				i++;
			else
				final = ft_strjoin_char(final, (*input)[i++]);
		}
		else if ((*input)[i] == '*' && q.flag)
		{
			final = ft_strjoin_char(final, '\\');
			final = ft_strjoin_char(final, '*');
			i++;
		}
		else if ((*input)[i] == '\\' && (!i || (*input)[i - 1] != '\\'))
			i++;
		else
			final = ft_strjoin_char(final, (*input)[i++]);
	}
	free(*input);
	*input = final;
}
