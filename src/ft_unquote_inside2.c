/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unquote_inside2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 03:55:23 by carlopez          #+#    #+#             */
/*   Updated: 2025/06/11 18:02:49 by carlotalcd       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_asterisk(int asterisk)
{
	if (!asterisk)
		return (1);
	return (0);
}

void	ft_asterisk_init(int *i, int *in_word, int *simp, int *asterisk)
{
	*i = 0;
	*in_word = 0;
	*simp = 0;
	*asterisk = 0;
}

void	ft_unquote_inside6(int *i, char **input, int *simp)
{
	if ((*input)[*i] == '\'')
		*simp = ft_simp(*simp);
	(*i)++;
}

void	ft_unquote_inside7(int *i, char **input, int *simp, char **unquoted)
{
	if (*simp || ((*input)[(*i) + 1]
		&& ((*input)[(*i) + 1] == '('
		|| (*input)[(*i) + 1] == ')' || (*input)[(*i) + 1] == '*')))
		*unquoted = ft_strjoin_char(*unquoted, (*input)[(*i)++]);
	else
		(*i)++;
}

void	ft_unquote_first(int (*i)[4], char **input, char **unquoted, int flag)
{
	ft_printf("i: %d, in_word: %d, simp: %d, asterisk: %d, flag: %d\n", (*i)[0], (*i)[1], (*i)[2], (*i)[3], flag);
	if (((*input)[(*i)[0]] == '\''
			|| (*input)[(*i)[0]] == '\"') && (flag || (*i)[3]) && (*i)[1])
	{
		ft_printf("uno\n");
		ft_unquote_inside(i[1], i[0], input, unquoted);
	}
	else if (ft_unquote_if((*i)[0], flag, input))
	{
		ft_printf("dos\n");
		ft_unquote_inside2(i[1], i[0], input, unquoted);
	}
	else if (ft_unquote_if2((*i)[0], input))
	{
		ft_printf("tres\n");
		ft_unquote_inside3(i[1], i[0], input, unquoted);
		(*i)[3] = ft_asterisk((*i)[3]);
	}
	else if (ft_unquote_if3((*i)[0], input))
	{
		ft_printf("cuatro\n");
		(*i)[3] = ft_unquote_inside4(i[1], i[2], (*i)[3]);
	}
	else if (ft_unquote_if4((*i)[0], flag, input))
	{
		ft_printf("cinco\n");
		(*i)[1] = ft_unquote_inside5(i[0], unquoted, input, i[2]);
	}
	else if ((*input)[(*i)[0]] == '\"' || (*input)[(*i)[0]] == '\'')
	{
		ft_printf("seis\n");
		ft_unquote_inside6(i[0], input, i[2]);
	}
	else if ((*input)[(*i)[0]] == '\\' || (*input)[(*i)[0]] == ';')
		ft_unquote_inside7(i[0], input, i[2], unquoted);
	else
	{
		ft_printf("une la %c\n", (*input)[(*i)[0]]);
		*unquoted = ft_strjoin_char(*unquoted, (*input)[(*i)[0]++]);
	}
}
