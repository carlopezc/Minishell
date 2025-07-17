/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:50:24 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/17 20:33:03 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_manage_sigint(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	return ;
}

void	ft_manage_shell_signals(void)
{
	signal(SIGINT, ft_manage_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	ft_csignal_child(int signal)
{
	(void)signal;
	exit(130);
}

void	ft_manage_child_signals(void)
{
	signal(SIGINT, ft_csignal_child);
	signal(SIGQUIT, SIG_IGN);
}
