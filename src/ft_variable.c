/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_variable.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 19:24:52 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/16 14:41:18 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

static int	ft_variable2(char **input, int *i,
		t_minishell **minishell, char **final)
{
	if ((*input)[++(*i)] == '?')
	{
		*final = ft_strjoin(*final, ft_itoa((*minishell)->status));
		return (0);
	}
	else
		*final = ft_strjoin(*final,
				ft_expand(*input, i, (*minishell)->env));
	return (1);
}

char	*ft_check_final(char *str)
{
	char	*final;
	int	i;

	final = NULL;
	i = 0;
	if (!str || (!ft_strchr(str, '\"') && (!ft_strchr(str, '\''))))
		return (str);
	while (str[i])	
	{
		if (str[i] == '\'' && (str[i - 1] == '\'' || str[i + 1] == '\''))
			i++;
		else if (str[i] == '\"' && (str[i - 1] == '\"' || str[i + 1] == '\"'))
			i++;
		else
			final = ft_strjoin_char(final, str[i++]);
	}
	return (final);
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
			{
				*input = final;
				return (1);
			}
		}
		else
			final = ft_strjoin_char(final, (*input)[i++]);
	}
	*input = ft_check_final(final);
	ft_printf("Tras ft_variable: %s\n", final);
	return (1);
}
