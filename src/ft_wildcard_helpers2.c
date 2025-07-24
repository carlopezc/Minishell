/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcard_helpers2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 03:53:21 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/24 03:55:07 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_handle_escaped_asterisk(char *str, int *i, int *flag, char **final)
{
	if (str[*i] && str[*i] == '\\' && str[*i + 1] == '*'
		&& !ft_find_asterisk(&str[*i]))
	{
		*final = ft_strjoin_char(*final, str[*i + 1]);
		*i += 2;
		*flag = 1;
		return (1);
	}
	return (0);
}

void	ft_handle_quotes_and_skip(char *str, int *i, t_quote *q, int *s_flag)
{
	if (!*i || str[*i - 1] != '\\')
	{
		if (q->flag)
			*s_flag = 1;
		ft_check_quote(q, str[*i]);
		if (*s_flag && !q->flag)
		{
			(*i)++;
			*s_flag = 0;
		}
	}
}

int	ft_handle_backslash_quotes(char *str, int i, t_quote *q, char **final)
{
	if (str[i] && str[i] == '\\' && q->type == '\'')
	{
		*final = ft_strjoin_char(*final, '\\');
		*final = ft_strjoin_char(*final, '\\');
		return (1);
	}
	return (0);
}
