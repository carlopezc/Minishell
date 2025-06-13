/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 19:26:24 by carlopez          #+#    #+#             */
/*   Updated: 2025/06/13 12:00:02 by carlotalcd       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

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

static char	*handle_escaped_char(char *unquoted, char *input, int i)
{
	return (ft_strjoin_char(unquoted, input[i]));
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
	if ((input[*i] == '\'' || input[*i] == '"') && in_word)
	{
		new = ft_strjoin_char(NULL, input[*i]);
		in_word = 0;
	}
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
	flags[2] = 0;
	unquoted = NULL;
	while (input[i])
	{
		if (i == 0 || input[i - 1] != '\\')
			unquoted = ft_strjoin(unquoted,
					handle_unescaped_char(input, &i, q,
						(int *[]){&flags[0], &flags[1], &flags[2]}));
		else
			unquoted = handle_escaped_char(unquoted, input, i);
		i++;
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
