/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcard_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 03:43:07 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/24 04:13:52 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_is_real_asterisk(char *str, int i, t_quote *q)
{
	return (str[i] && str[i] == '*' && !q->flag
		&& (!i || str[i - 1] != '\\'));
}

int	ft_process_asterisk_pattern(char *str,
		int *i, char **str_final, t_token **tmp)
{
	if (!ft_get_pattern(str, i, str_final, tmp))
		return (0);
	return (1);
}

int	ft_handle_quotes_in_word(char *str, int i, char **final)
{
	if (str[i] && (str[i] == '\'' || str[i] == '\"')
		&& str[i - 1] != '\\' && ft_find_asterisk_in_word(str, i))
	{
		*final = ft_strjoin_char(*final, str[i]);
		return (1);
	}
	return (0);
}

int	ft_handle_normal_chars(char *str, int i, t_quote *q, char **final)
{
	if (str[i] && !q->flag
		&& ((str[i] == '*' && !ft_find_asterisk(&str[i]))
			|| (!ft_is_quote(str[i]) && !ft_find_asterisk(&str[i]))))
	{
		*final = ft_strjoin_char(*final, str[i]);
		return (1);
	}
	if (str[i] && q->flag && str[i] != '\'' && str[i] != '\"')
	{
		*final = ft_strjoin_char(*final, str[i]);
		return (1);
	}
	return (0);
}
