/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unquote_if.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 03:52:55 by carlopez          #+#    #+#             */
/*   Updated: 2025/06/01 03:57:29 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_unquote_if(int i, int flag, char **input)
{
	if (((*input)[i] == '\"')
		&& flag && ((ft_strchr(&(*input)[i], ' '))
		&& (ft_strchr(&(*input)[i + 1], '\"'))
			&& (ft_strchr(&(*input)[i], ' ')
			< ft_strchr(&(*input)[i + 1], '\"'))))
		return (1);
	return (0);
}

int	ft_unquote_if2(int i, char **input)
{
	if (((*input)[i] == '\"')
		&& ((ft_strchr(&(*input)[i], '*'))
		&& (ft_strchr(&(*input)[i + 1], '\"'))
			&& (ft_strchr(&(*input)[i], '*')
			< ft_strchr(&(*input)[i + 1], '\"'))))
		return (1);
	return (0);
}

int	ft_unquote_if3(int i, char **input)
{
	if (((*input)[i] == '\'')
		&& ((ft_strchr(&(*input)[i], '*'))
		&& (ft_strchr(&(*input)[i + 1], '\''))
			&& (ft_strchr(&(*input)[i], '*')
			< ft_strchr(&(*input)[i + 1], '\''))))
		return (1);
	return (0);
}

int	ft_unquote_if4(int i, int flag, char **input)
{
	if (((*input)[i] == '\'')
		&& flag && ((ft_strchr(&(*input)[i], ' '))
		&& (ft_strchr(&(*input)[i + 1], '\''))
			&& (ft_strchr(&(*input)[i], ' ')
			< ft_strchr(&(*input)[i + 1], '\''))))
		return (1);
	return (0);
}

int	ft_simp(int simp)
{
	if (simp)
		return (0);
	return (1);
}
