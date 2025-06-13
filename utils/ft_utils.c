/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:47:17 by lbellmas          #+#    #+#             */
/*   Updated: 2025/06/13 11:58:18 by carlotalcd       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "../header/ft_minishell.h"

char	*ft_quit_quotes(char **s_input, t_minishell **minishell)
{
	int		i;
	char	*input;
	int		flag;

	i = 0;
	flag = 0;
	while (s_input[i])
	{
		ft_variable(&s_input[i], minishell);
		ft_unquote(&s_input[i], flag);
		if (!ft_strncmp(s_input[i], "export", 7)
			|| !ft_strncmp(s_input[i], "env", 4))
			flag = 1;
		if (ft_check_operator(s_input[i]))
			flag = 0;
		i++;
	}
	input = ft_create_array(s_input);
	return (input);
}

int	ft_simp(int simp)
{
	if (simp)
		return (0);
	return (1);
}

void	ft_clear_split(char **split)
{
	int	p;

	if (!split)
		return ;
	p = 0;
	while (split[p])
	{
		free(split[p]);
		p++;
	}
	free(split);
}
