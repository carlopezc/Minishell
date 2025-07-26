/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_spaces.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 18:14:34 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/26 03:28:25 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_skip_spaces(char *input, int *i)
{
	if (!input)
		return ;
	while (input[*i] && input[*i] == ' ')
		(*i)++;
	return ;
}

int	ft_parse_spaces(char **input)
{
	int		i;
	t_quote	q;
	char	*final;
	char	*tmp;
	char	prev;

	i = 0;
	tmp = *input;
	ft_init_quote(&q);
	final = NULL;
	while (tmp[i] && tmp[i] == ' ')
		i++;
	prev = tmp[i];
	while (tmp[i])
	{
		if ((tmp[i] == '\'' || tmp[i] == '\"') && (!i || tmp[i - 1] != '\\'))
			ft_check_quote(&q, tmp[i]);
		if (tmp[i] == ' ' && ((prev
					&& (prev != '(' && prev != ')'))
				|| ft_check_operator(&(tmp[i]) + 1)))
			final = ft_strjoin_char(final, tmp[i++]);
		while (tmp[i] && tmp[i] == ' ' && !q.flag)
			i++;
		if (tmp[i])
		{
			prev = tmp[i];
			final = ft_strjoin_char(final, tmp[i++]);
		}
		if (!final)
			return (0);
	}
	free(*input);
	*input = final;
	return (1);
}
