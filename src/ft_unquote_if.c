/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unquote_if.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 03:52:55 by carlopez          #+#    #+#             */
/*   Updated: 2025/06/13 10:39:43 by carlotalcd       ###   ########.fr       */
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

int	ft_unquote_if5(int i, int flag, char **input)
{
	if (((*input)[i] == '\"')
		&& flag && ((ft_strchr(&(*input)[i], ' '))
		&& (ft_strchr(&(*input)[i + 1], '\"'))
			&& (ft_strchr(&(*input)[i], ' ')
			< ft_strchr(&(*input)[i + 1], '\"'))))
		return (1);
	return (0);
}
