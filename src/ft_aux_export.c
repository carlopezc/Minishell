/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_aux_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 02:38:38 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/26 04:29:34 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_aux_change_value(char **name, char **str_value, t_env **node)
{
	int		value;

	value = 0;
	if (!ft_strncmp(*name, "SHLVL", 6))
	{
		value = ft_atoi(*str_value);
		if (value > 999)
			value = 1;
		ft_safe_free((void **)&((*node)->value));
		(*node)->value = ft_itoa(value);
		ft_safe_free((void **)str_value);
	}
	else
	{
		ft_safe_free((void **)&((*node)->value));
		(*node)->value = *str_value;
	}
}

void	ft_change_value(char *str, t_env **node)
{
	char	*equal;
	char	*str_value;
	char	*name;
	char	*value;

	equal = ft_strchr(str, '=');
	name = ft_get_name(str);
	str_value = ft_get_value(str);
	value = NULL;
	if (*(equal - 1) == '+')
	{
		value = (*node)->value;
		(*node)->value = ft_strjoin(value, str_value);
		ft_safe_free((void **)&value);
		ft_safe_free((void **)&str_value);
	}
	else
		ft_aux_change_value(&name, &str_value, node);
	ft_safe_free((void **)&name);
	return ;
}
