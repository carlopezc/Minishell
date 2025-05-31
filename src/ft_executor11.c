/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor11.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 18:18:41 by lbellmas          #+#    #+#             */
/*   Updated: 2025/05/31 19:02:06 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"
#include <fcntl.h>
#include <sys/wait.h>

static t_token	*ft_redir2(t_pipex *pipex, t_token *save,
		t_token_type type, int count)
{
	while (save && save->type == type)
	{
		if (type == REDIR_IN)
			pipex->docs_in[count] = open(save->str, O_RDONLY);
		else if (type == APPEND)
			pipex->docs_out[count]
				= open(save->str, O_WRONLY | O_APPEND | O_CREAT, 0777);
		else
		{
			unlink(save->str);
			pipex->docs_out[count] = open(save->str, O_WRONLY | O_CREAT, 0777);
		}
		count++;
		save = save->next;
	}
	return (save);
}

int	ft_subint_out(t_pipex *pipex, int add)
{
	int	*new;
	int	count;

	count = 0;
	while (pipex->docs_out[count])
		count++;
	new = (int *)malloc(sizeof(int) * (count + add));
	count = 0;
	while (pipex->docs_out[count])
	{
		new[count] = pipex->docs_out[count];
		count++;
	}
	free(pipex->docs_out);
	pipex->docs_out = new;
	return (count);
}

int	ft_subint_in(t_pipex *pipex, int add)
{
	int	*new;
	int	count;

	count = 0;
	while (pipex->docs_in[count])
		count++;
	new = (int *)malloc(sizeof(int) * (count + add));
	count = 0;
	while (pipex->docs_in[count])
	{
		new[count] = pipex->docs_in[count];
		count++;
	}
	free(pipex->docs_in);
	pipex->docs_in = new;
	return (count);
}

static int	ft_redir3(t_pipex *pipex, t_token_type type, int count)
{
	if (type == REDIR_IN && pipex->docs_in)
		count = ft_subint_in(pipex, count);
	else if (type != REDIR_IN && pipex->docs_out)
		count = ft_subint_out(pipex, count);
	else if (type == REDIR_IN)
	{
		pipex->docs_in = (int *)malloc(count * sizeof(int));
		count = 0;
	}
	else
	{
		pipex->docs_out = (int *)malloc(count * sizeof(int));
		count = 0;
	}
	if (!pipex->docs_in && !pipex->docs_out)
		exit(0);
	return (count);
}

t_token	*ft_redir(t_token *save, t_token_type type, t_pipex *pipex)
{
	t_token	*temp;
	int		count;

	temp = save;
	count = 1;
	while (temp && temp->type == type)
	{
		count++;
		temp = temp->next;
	}
	temp = save;
	count = ft_redir3(pipex, type, count);
	save = ft_redir2(pipex, save, type, count);
	while (temp && temp->type == type)
	{
		count++;
		temp = temp->next;
	}
	if (type == REDIR_IN)
		pipex->docs_in[count] = 0;
	else
		pipex->docs_out[count] = 0;
	return (save);
}
