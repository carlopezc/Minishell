/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_elements2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 03:12:59 by lbellmas          #+#    #+#             */
/*   Updated: 2025/07/26 03:13:17 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

char	*ft_get_value(char *str)
{
	int		i;
	char	*value;
	char	*equal;
	char	*tmp;

	i = 0;
	equal = ft_strchr(str, '=');
	if (!equal)
		return (NULL);
	else if (!*(equal + 1))
		value = ft_strdup("");
	else
	{
		i++;
		value = ft_strdup("");
		while (equal[i])
		{
			if (ft_get_value_join(&value, equal, &i, &tmp) == 0)
				return (0);
		}
	}
	return (ft_clean_value(value));
}

char	*ft_get_name(char *str)
{
	int		i;
	int		init;
	char	*name;

	i = 0;
	init = 0;
	if (!str)
		return (NULL);
	if (str[i] && (str[i] == '\'' || str[i] == '\"'))
	{
		init++;
		i++;
	}
	while (str && str[i])
	{
		if (str[i] == '\'' || str[i] == '\"' || str[i] == '=')
			break ;
		else if (str[i] == '+' && str[i + 1] && str[i + 1] == '=')
			break ;
		i++;
	}
	name = ft_substr(str, init, i - init);
	return (name);
}
