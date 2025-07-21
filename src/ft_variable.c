/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_variable.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 19:24:52 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/21 19:39:27 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

static int	ft_variable2(char **input, int *i,
		t_minishell **minishell, char **final)
{
	char	*old;
	char	*expanded;

	old = NULL;
	expanded = NULL;
	if ((*input)[++(*i)] == '?')
	{
		old = *final;
		expanded = ft_itoa((*minishell)->status);
		*final = ft_strjoin(*final, expanded);
		free(old);
		free(expanded);
		return (0);
	}
	else
	{
		old = *final;
		expanded = ft_expand(*input, i, (*minishell)->env);
		*final = ft_strjoin(*final,
				expanded);
		free(old);
		free(expanded);
	}
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
		if (str[i] == '\'' && (!i || (str[i - 1] == '\'' || str[i + 1] == '\'')))
			i++;
		else if (str[i] == '\"' && (!i || (str[i - 1] == '\"' || str[i + 1] == '\"')))
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
		if (!i || (!(*input)[i - 1] || (*input)[i - 1] != '\\'))
			ft_check_quote(&quote, (*input)[i]);
		if ((*input)[i] == '$' && (quote.type != '\''
				&& (!i || (!(*input)[i - 1] || (*input)[i - 1] != '\\'))))
		{
			if (!ft_variable2(input, &i, minishell, &final))
			{
				free(*input);
				*input = final;
				return (1);
			}
		}
		else
			final = ft_strjoin_char(final, (*input)[i++]);
	}
	free(*input);
	*input = ft_check_final(final);
	return (1);
}
