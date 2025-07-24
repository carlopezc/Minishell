/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_aux_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 17:26:41 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/24 17:27:51 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_signals_here(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit(130);
}

void	ft_manage_here_signals(void)
{
	signal(SIGINT, ft_signals_here);
	signal(SIGQUIT, SIG_IGN);
}

void	ft_normal_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
