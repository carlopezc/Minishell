/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 19:26:24 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/31 20:04:19 by carlopez         ###   ########.fr       */
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
	int		i;
	int		in_word;
	int		simp;
	int		asterisk;

	i = 0;
	in_word = 0;
	simp = 0;
	asterisk = 0;
	unquoted = NULL;
	while ((*input)[i])
	{
		if (!i || (!(*input)[i - 1] || (*input)[i - 1] != '\\'))
		{
			if (((*input)[i] == '\''
				|| (*input)[i] == '\"') && (flag || asterisk) && in_word)
			{
				in_word = 0;
				unquoted = ft_strjoin_char(unquoted, (*input)[i++]);
			}
			else if (((*input)[i] == '\"')
				&& flag && ((ft_strchr(&(*input)[i], ' '))
				&& (ft_strchr(&(*input)[i + 1], '\"'))
				&& (ft_strchr(&(*input)[i], ' ')
				< ft_strchr(&(*input)[i + 1], '\"'))))
			{
				in_word = 1;
				unquoted = ft_strjoin_char(unquoted, (*input)[i++]);
			}
			else if (((*input)[i] == '\"')
				&& ((ft_strchr(&(*input)[i], '*'))
				&& (ft_strchr(&(*input)[i + 1], '\"'))
				&& (ft_strchr(&(*input)[i], '*')
				< ft_strchr(&(*input)[i + 1], '\"'))))
			{
				if (!asterisk)
					asterisk = 1;
				else
					asterisk = 0;
				in_word = 1;
				unquoted = ft_strjoin_char(unquoted, (*input)[i++]);
			}
			else if (((*input)[i] == '\'')
				&& ((ft_strchr(&(*input)[i], '*'))
				&& (ft_strchr(&(*input)[i + 1], '\''))
				&& (ft_strchr(&(*input)[i], '*')
				< ft_strchr(&(*input)[i + 1], '\''))))
			{
				if (!asterisk)
					asterisk = 1;
				else
					asterisk = 0;
				in_word = 1;
				unquoted = ft_strjoin_char(unquoted, (*input)[i++]);
				if (simp)
					simp = 0;
				else
					simp = 1;
			}
			else if (((*input)[i] == '\'')
				&& flag && ((ft_strchr(&(*input)[i], ' '))
				&& (ft_strchr(&(*input)[i + 1], '\''))
				&& (ft_strchr(&(*input)[i], ' ')
				< ft_strchr(&(*input)[i + 1], '\''))))
			{
				in_word = 1;
				unquoted = ft_strjoin_char(unquoted, (*input)[i++]);
				if (simp)
					simp = 0;
				else
					simp = 1;
			}
			else if ((*input)[i] == '\"' || (*input)[i] == '\'')
			{
				if ((*input)[i] == '\'')
				{
					if (simp)
						simp = 0;
					else
						simp = 1;
				}
				i++;
			}
			else if ((*input)[i] == '\\' || (*input)[i] == ';')
			{
				if (simp || ((*input)[i + 1]
					&& ((*input)[i + 1] == '(' || (*input)[i + 1] == ')')))
					unquoted = ft_strjoin_char(unquoted, (*input)[i++]);
				else
					i++;
			}
			else
				unquoted = ft_strjoin_char(unquoted, (*input)[i++]);
		}
		else
			unquoted = ft_strjoin_char(unquoted, (*input)[i++]);
	}
	*input = unquoted;
}
