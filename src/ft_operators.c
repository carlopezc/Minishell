/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_operators.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 17:11:06 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/24 17:12:23 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

t_token	*ft_and(t_pipex *pipex, t_minishell *shell, t_token *save)
{
	int	brackets;

	brackets = pipex->brackets_count;
	ft_check_exit(pipex, shell);
	if (shell->status != 0)
	{
		while (save && (save->type != AND || pipex->brackets_count <= brackets)
			&& (save->type != OR || brackets != pipex->brackets_count))
		{
			if (save->type == O_BRACKET)
				pipex->brackets_count += 1;
			if (save->type == C_BRACKET)
				pipex->brackets_count -= 1;
			save = save->next;
		}
	}
	return (save);
}

t_token	*ft_or(t_pipex *pipex, t_minishell *shell, t_token *save)
{
	int	brackets;

	brackets = pipex->brackets_count;
	ft_check_exit(pipex, shell);
	if (shell->status == 0)
	{
		while (save && (save->type != AND
				|| !(pipex->brackets_count <= brackets))
			&& (save->type != OR || brackets != pipex->brackets_count))
		{
			if (save->type == O_BRACKET)
				pipex->brackets_count += 1;
			if (save->type == C_BRACKET)
				pipex->brackets_count -= 1;
			save = save->next;
		}
	}
	return (save);
}
