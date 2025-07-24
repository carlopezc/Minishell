/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_find.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 06:40:15 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/24 06:40:32 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

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
