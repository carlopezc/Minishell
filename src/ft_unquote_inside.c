/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unquote_inside.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 03:54:30 by carlopez          #+#    #+#             */
/*   Updated: 2025/06/01 03:55:06 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_unquote_inside(int *in_word, int *i, char **input, char **unquoted)
{
	*in_word = 0;
	*unquoted = ft_strjoin_char(*unquoted, (*input)[(*i)++]);
}

void	ft_unquote_inside2(int *in_word, int *i, char **input, char **unquoted)
{
	*in_word = 1;
	*unquoted = ft_strjoin_char(*unquoted, (*input)[(*i)++]);
}

void	ft_unquote_inside3(int *in_word, int *i, char **input, char **unquoted)
{
	*in_word = 1;
	*unquoted = ft_strjoin_char(*unquoted, (*input)[(*i)++]);
}

int	ft_unquote_inside4(int *in_word, int *simp, int asterisk)
{
	*in_word = 1;
	*simp = ft_simp(*simp);
	return (ft_asterisk(asterisk));
}

int	ft_unquote_inside5(int *i, char **unquoted, char **input, int *simp)
{
	*unquoted = ft_strjoin_char(*unquoted, (*input)[(*i)++]);
	*simp = ft_simp(*simp);
	return (1);
}
