/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 19:26:24 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/14 19:18:48 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"
/*
static int	should_add_literal(char c, int simp, char next)
{
	if (c == '\\' || c == ';')
	{
		if (simp)
			return (1);
		if (next == '(' || next == ')')
			return (1);
	}
	return (0);
}

static char	*handle_unescaped_char(char *input, int *i, t_quote *q,
	int *flags[3])
{
	char	*new;
	int		simp;
	int		asterisk;
	int		in_word;

	new = NULL;
	simp = *flags[0];
	asterisk = *flags[1];
	in_word = *flags[2];
	ft_check_quote(q, input[*i]);
	while (input[*i] && input[*i] == ' ')
		(*i)++;
	//if ((input[*i] == '\'' || input[*i] == '"') && in_word)
	//	new = ft_strjoin_char(NULL, input[*i]);
	if (input[*i] == ' ' && (!*i || input[*i - 1] != '\\') && in_word)
		in_word = 0;
	else if (should_add_literal(input[*i], simp, input[*i + 1]))
		new = ft_strjoin_char(NULL, input[*i]);
	else
		new = ft_strjoin_char(NULL, input[*i]);
	*flags[0] = simp;
	*flags[1] = asterisk;
	*flags[2] = in_word;
	return (new);
}

static char	*ft_process_unquote_loop(char *input, t_quote *q)
{
	char	*unquoted;
	int		i;
	int		flags[3];

	i = 0;
	flags[0] = 0;
	flags[1] = 0;
	flags[2] = 1;
	unquoted = NULL;
	while (input[i])
	{
		if (input[i] && input[i] == ' ' && flags[2])
			flags[2] = 0;
		while (input[i] && input[i] == ' ')
			i++;
		flags[2] = 1;
		if (!input[i])
			break ;
		if (ft_is_quote(input[i]) && (!i || input[i - 1] != '\\'))
			i++;
		else
			unquoted = ft_strjoin_char(unquoted, input[i]);
	}
	return (unquoted);
}

void	ft_unquote(char **input, int flag)
{
	t_quote	q;
	char	*unquoted;

	(void)flag;
	ft_init_quote(&q);
	unquoted = ft_process_unquote_loop(*input, &q);
	if (!unquoted)
		*input = ft_strdup("");
	else
		*input = unquoted;
}
*/

void	ft_unquote(char **input, int flag)
{
	char	*unquoted;
	int		i;
	int		in_word;
	int		simp;
	int		asterisk;

	i = 0;
	in_word = 0;
	simp = 0;
	asterisk = 0;
	unquoted = NULL;
	while ((*input)[i])
	{
		if (!i || (!(*input)[i - 1] || (*input)[i - 1] != '\\'))
		{
			if (((*input)[i] == '\''
				|| (*input)[i] == '\"') && (flag || asterisk) && in_word)
			{
				in_word = 0;
				unquoted = ft_strjoin_char(unquoted, (*input)[i++]);
			}
			else if (((*input)[i] == '\"')
				&& flag && ((ft_strchr(&(*input)[i], ' '))
				&& (ft_strchr(&(*input)[i + 1], '\"'))
				&& (ft_strchr(&(*input)[i], ' ')
				< ft_strchr(&(*input)[i + 1], '\"'))))
			{
				in_word = 1;
				unquoted = ft_strjoin_char(unquoted, (*input)[i++]);
			}
			else if (((*input)[i] == '\"')
				&& ((ft_strchr(&(*input)[i], '*'))
				&& (ft_strchr(&(*input)[i + 1], '\"'))
				&& (ft_strchr(&(*input)[i], '*')
				< ft_strchr(&(*input)[i + 1], '\"'))))
			{
				if (!asterisk)
					asterisk = 1;
				else
					asterisk = 0;
				in_word = 1;
				unquoted = ft_strjoin_char(unquoted, (*input)[i++]);
			}
			else if (((*input)[i] == '\'')
				&& ((ft_strchr(&(*input)[i], '*'))
				&& (ft_strchr(&(*input)[i + 1], '\''))
				&& (ft_strchr(&(*input)[i], '*')
				< ft_strchr(&(*input)[i + 1], '\''))))
			{
				if (!asterisk)
					asterisk = 1;
				else
					asterisk = 0;
				in_word = 1;
				unquoted = ft_strjoin_char(unquoted, (*input)[i++]);
				if (simp)
					simp = 0;
				else
					simp = 1;
			}
			else if (((*input)[i] == '\'')
				&& flag && ((ft_strchr(&(*input)[i], ' '))
				&& (ft_strchr(&(*input)[i + 1], '\''))
				&& (ft_strchr(&(*input)[i], ' ')
				< ft_strchr(&(*input)[i + 1], '\''))))
			{
				in_word = 1;
				unquoted = ft_strjoin_char(unquoted, (*input)[i++]);
				if (simp)
					simp = 0;
				else
					simp = 1;
			}
			else if ((*input)[i] == '\"' || (*input)[i] == '\'')
			{
				if ((*input)[i] == '\'')
				{
					if (simp)
						simp = 0;
					else
						simp = 1;
				}
				i++;
			}
			else if ((*input)[i] == '\\' || (*input)[i] == ';')
			{
				ft_printf("caracter\n");
				if (simp || ((*input)[i + 1]
					&& ((*input)[i + 1] == '(' || (*input)[i + 1] == ')')
					&& ((*input)[i + 1] == '(' || (*input)[i + 1] == ')')) || (((*input)[i + 1] == '\"') && ft_strchr(&(*input)[i], '*') && ft_strchr(&(*input)[i + 2], '\"') && (ft_strchr(&(*input)[i], '*') < ft_strchr(&(*input)[i + 2], '\"'))))
					unquoted = ft_strjoin_char(unquoted, (*input)[i++]);
				else
					i++;
			}
			else
				unquoted = ft_strjoin_char(unquoted, (*input)[i++]);
		}
		else
			unquoted = ft_strjoin_char(unquoted, (*input)[i++]);
	}
	*input = unquoted;
	if (!*input)
		*input = ft_strdup("");
}
