/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 18:28:50 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/29 18:30:14 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_count_brackets(char *str)
{
	int	i;
	int	o_bracket;
	int	c_bracket;

	o_bracket = 0;
	c_bracket = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '(' && (i > 0 && (!str[i - 1] || str[i - 1] != '\\')))
			o_bracket++;
		else if (str[i] == ')' && (i > 0
				&& (!str[i - 1] || str[i - 1] != '\\')))
			c_bracket++;
		i++;
	}
	if (o_bracket == c_bracket)
		return (1);
	return (0);
}

int	ft_count_quotes(char *str)
{
	int	i;
	int	double_quotes;
	int	simple_quotes;

	i = 0;
	simple_quotes = 0;
	double_quotes = 0;
	while (str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
		{
			if (!str[i - 1] || (str[i - 1] && str[i - 1] != '\\'))
			{
				if (str[i] == '\"')
					double_quotes++;
				else
					simple_quotes++;
			}
		}
		i++;
	}
	if (!(double_quotes % 2) && !(simple_quotes % 2))
		return (1);
	return (0);
}
