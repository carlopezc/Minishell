/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:50:24 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/24 16:21:14 by lbellmas         ###   ########.fr       */
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

void	ft_manage_sigint(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	control_c = 130;
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

void	ft_normal_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	ft_manage_child_signals(void)
{
	signal(SIGINT, ft_csignal_child);
	signal(SIGQUIT, SIG_IGN);
}
