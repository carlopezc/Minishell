/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_variable.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 19:24:52 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/31 19:58:45 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

static int	ft_variable2(char **input, int *i,
		t_minishell **minishell, char **final)
{
	if ((*input)[++(*i)] == '?')
		return (ft_printf("%d\n", (*minishell)->status), 0);
	else
		*final = ft_strjoin(*final,
				ft_expand(*input, i, (*minishell)->env));
	return (1);
}

int	ft_variable(char **input, t_minishell **minishell)
{
	t_quote	quote;
	int		i;
	char	*final;

	ft_init_quote(&quote);
	i = 0;
	final = NULL;
	if (!*input || !(*input)[0] || !ft_strchr(*input, '$'))
		return (0);
	while ((*input)[i])
	{
		if (!(*input)[i - 1] || (*input)[i - 1] != '\\')
			ft_check_quote(&quote, (*input)[i]);
		if ((*input)[i] == '$' && (quote.type != '\''
				&& (!(*input)[i - 1] || (*input)[i - 1] != '\\')))
		{
			if (!ft_variable2(input, &i, minishell, &final))
				return (1);
		}
		else
			final = ft_strjoin_char(final, (*input)[i++]);
	}
	*input = final;
	return (1);
}
