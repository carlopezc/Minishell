/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:15:42 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/19 12:44:36 by carlotalcd       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

char	*ft_simp_unquote(char *input, int *i, t_quote q)
{
	char	*unquoted;

	unquoted = NULL;
	if (input[*i] != '\'')
		return (NULL);
	if (!q.flag || (q.type == '\''))
		(*i)++;	//me salto primera comilla simple
	else
		unquoted = ft_strjoin_char(unquoted, input[(*i)++]);
	while (input[*i] && input[*i] != '\''/* && (!*i || input[*i - 1] != '\\')*/)
	{
		if (input[*i] == '*')
			unquoted = ft_strjoin_char(unquoted, '\\');
		unquoted = ft_strjoin_char(unquoted, input[(*i)++]);
	}
	if (!q.flag || (q.type == '\''))
		(*i)++;	//me salto segunda comilla simple
	else
		unquoted = ft_strjoin_char(unquoted, input[(*i)++]);
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

void	ft_unquote_except(char **input)
{
	(void)input;
	return ;
}

void	ft_unquote(char **input, int flag)
{
	int	i;
	t_quote	q;
	int	in_word;
	char	*final;

	i = 0;
	ft_init_quote(&q);
	final = NULL;
	in_word = 1;
	if (flag)
		return (ft_unquote_except(input));
	int count = 0;
	//falla <<'$USER'  
	while ((*input)[i])	
	{
		ft_printf("i = %i, count = %i, input = %s, len = %i\n", i, count, *input, ft_strlen(*input));
		count++;
		if ((*input)[i] == '\'' && (!i || (*input)[i - 1] != '\\'))
			final = ft_strjoin(final, ft_simp_unquote(*input, &i, q));
		ft_check_in_word(&in_word, (*input)[i]);
		if (((*input)[i] == '\'' || (*input)[i] == '\"') && (!i || (*input)[i - 1] != '\\'))
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
		ft_printf("hola\n");
	}
	*input = final;
}
