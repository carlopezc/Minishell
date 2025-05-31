/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_variable.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 19:24:52 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/30 21:32:35 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_variable(char **input, t_minishell **minishell)
{
	char	*dollar;
	t_quote	quote;
	int		i;
	char	*final;

	ft_init_quote(&quote);
	i = 0;
	final = NULL;
	if (!*input || !(*input)[0])
		return ;
	dollar = ft_strchr(*input, '$');
	if (!dollar)
		return ;
	while ((*input)[i])
	{
		if (!(*input)[i - 1] || ((*input)[i - 1]
				&& (*input)[i - 1] != '\\'))
			ft_check_quote(&quote, (*input)[i]);
		if ((*input)[i] == '$' && (quote.type != '\''
				&& (!(*input)[i - 1] || ((*input)[i - 1]
					&& (*input)[i - 1] != '\\'))))
		{
			if ((!(*input)[i - 1]) || ((*input)[i - 1]
					&& (*input)[i - 1] != '\\'))
			{
				i++;
				if ((*input)[i] == '?')
				{
					ft_printf("%d\n", (*minishell)->status);
					return ;
				}
				else
					final = ft_strjoin(final,
							ft_expand(*input, &i, (*minishell)->env));
			}
			else
			{
				final = ft_strjoin_char(final, (*input)[i]);
				if ((*input)[i + 1]
					&& ((*input)[i + 1] != '\'' && (*input)[i + 1] != '\"'))
					i--;
			}
		}
		else
			final = ft_strjoin_char(final, (*input)[i++]);
	}
	*input = final;
	return ;
}
