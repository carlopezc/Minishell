/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:05:18 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/21 20:19:00 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_check_quote(t_quote *quote, char c)
{
	if (c == '\'' || c == '\"')
	{
		if (quote->flag == 0)
		{
			quote->flag = 1;
			quote->type = c;
		}
		else if (quote->type != c)
			return ;
		else if (quote->type == c)
		{
			quote->flag = 0;
			quote->type = 0;
		}
	}
	return ;
}

int	ft_check_operator(char *input)
{
	if (!input || input[0] == '\0')
		return (0);
	else if (*input == '(' || *input == ')')
		input = input + 1;
	if (!ft_strncmp(input, "||", 2) || !ft_strncmp(input, "|", 1))
		return (1);
	else if (!ft_strncmp(input, ">>", 2) || !ft_strncmp(input, ">", 1))
		return (1);
	else if (!ft_strncmp(input, "<<", 2) || !ft_strncmp(input, "<", 1))
		return (1);
	else if (!ft_strncmp(input, "&&", 2))
		return (1);
	else
		return (0);
}
