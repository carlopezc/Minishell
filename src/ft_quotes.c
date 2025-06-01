/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 19:26:24 by carlopez          #+#    #+#             */
/*   Updated: 2025/06/01 03:57:06 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
			|| !ft_strncmp(s_input[i], "env", 4)
			|| !ft_strncmp(s_input[i], "echo", 5))
			flag = 1;
		if (ft_check_operator(s_input[i]))
			flag = 0;
		i++;
	}
	input = ft_create_array(s_input);
	return (input);
}

void	ft_unquote(char **input, int flag)
{
	char	*unquoted;
	int		i[4];

	ft_asterisk_init(&i[0], &i[1], &i[2], &i[3]);
	unquoted = NULL;
	while ((*input)[i[0]])
	{
		if (!i[0] || (!(*input)[i[0] - 1] || (*input)[i[0] - 1] != '\\'))
			ft_unquote_first(&i, input, &unquoted, flag);
		else
			unquoted = ft_strjoin_char(unquoted, (*input)[i[0]++]);
	}
	*input = unquoted;
}
