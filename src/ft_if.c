/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_if.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 22:10:42 by lbellmas          #+#    #+#             */
/*   Updated: 2025/07/25 22:25:05 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_if_no_next(t_env **tmp, t_env **list)
{
	if (*tmp && !(*tmp)->next)
	{
		ft_safe_free((void **)&(*tmp)->name);
		ft_safe_free((void **)&(*tmp)->value);
		ft_safe_free((void **)tmp);
		*list = NULL;
		list = NULL;
		return (0);
	}
	return (1);
}
