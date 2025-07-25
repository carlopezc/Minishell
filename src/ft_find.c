/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_find.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 06:40:15 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/25 14:43:15 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_first_or_last(char *str, int i)
{
	int	cpy;

	cpy = i;
	while (i > 0)
	{
		if (str[i] && (str[i] != ' ' && str[i] != '('))
		{
			while (str[cpy])
			{
				if (str[cpy] != ' ' && str[cpy] != ')')
					return (0);
				cpy++;
			}
		}
		i--;
	}
	return (1);
}

int	ft_find_asterisk_in_word(const char *str, int index)
{
	int	start;
	int	end;
	int	i;

	start = index;
	end = index;
	while (start > 0 && str[start - 1] != ' ')
		start--;
	while (str[end] && str[end] != ' ')
		end++;
	i = start;
	while (i < end)
	{
		if (str[i] == '*')
			return (1);
		i++;
	}
	return (0);
}
