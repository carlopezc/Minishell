/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:44:53 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/08 17:28:42 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_check_quotes(char *str)
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


}

int	ft_parsing(char **input)
{
	char	*src;
	int	i;
	int	simple_quotes;
	int	double_quotes;

	i = 0;
	if (!input || !*input)
		return (0);
	src = *input;
	if (!ft_check_quotes(src))
		return (0);
	//Esto lo metere en un check quotes
	return (1);
}
