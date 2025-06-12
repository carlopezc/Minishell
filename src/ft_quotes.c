/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 19:26:24 by carlopez          #+#    #+#             */
/*   Updated: 2025/06/12 17:41:28 by carlotalcd       ###   ########.fr       */
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
			|| !ft_strncmp(s_input[i], "env", 4))
			flag = 1;
		if (ft_check_operator(s_input[i]))
			flag = 0;
		i++;
	}
	input = ft_create_array(s_input);
	return (input);
}
/*
void	ft_unquote(char **input, int flag)
{
	t_quote	quote;
	char	*unquoted;
	int 	i;

	ft_init_quote(&quote);
	unquoted = NULL;
	i = 0;
	while ((*input)[i])
	{
		if (!(*input)[i - 1] || (*input)[i - 1] != '\\')
		{
			ft_check_quote(&quote, (*input)[i]);
			if (quote.flag && flag)
				unquoted = ft_strjoin_char(unquoted, (*input)[i]);
			else if (quote.flag && ((*input)[i] == '\'' || (*input)[i] == '\"'))
				unquoted = ft_strjoin_char(unquoted, (*input)[i]);

			// general de check al unquoted si ha fallado malloc salgo
		}
		else
			unquoted = ft_strjoin_char(unquoted, (*input)[i]);
			// si falla return
			// caso comillas escapadas lo escribe tal cual
	}



}
	*/

void	ft_unquote(char **input, int flag)
{
	char	*unquoted;
	int		i;
	int		in_word;
	int		simp;
	int		asterisk;
	t_quote quote;

	i = 0;
	in_word = 0;
	simp = 0;
	asterisk = 0;
	unquoted = NULL;
	ft_init_quote(&quote);
	while ((*input)[i])
	{
		if (!i || (!(*input)[i - 1] || (*input)[i - 1] != '\\'))
		{
			ft_check_quote(&quote, (*input)[i]);
			if (((*input)[i] == '\''
				|| (*input)[i] == '\"') && (flag || asterisk) && in_word)
			{
				// aqui no distingo entre simples y dobles
				in_word = 0;
				unquoted = ft_strjoin_char(unquoted, (*input)[i]);
			}
			else if (((*input)[i] == '\"')
				&& flag && ((ft_strchr(&(*input)[i], ' '))
				&& (ft_strchr(&(*input)[i + 1], '\"'))
				&& (ft_strchr(&(*input)[i], ' ')
				< ft_strchr(&(*input)[i + 1], '\"'))))
			{
				in_word = 1;
				unquoted = ft_strjoin_char(unquoted, (*input)[i]);
			}
			else if (((*input)[i] == '\"')
				&& ((ft_strchr(&(*input)[i], '*'))
				&& (ft_strchr(&(*input)[i + 1], '\"'))
				&& (ft_strchr(&(*input)[i], '*')
				< ft_strchr(&(*input)[i + 1], '\"'))))
			{
				//lo puedo meter todos los ifs en estructuras de mas alante que, y le paso los dos caracteres
				if (!asterisk)
					asterisk = 1;
				else
					asterisk = 0;
				in_word = 1;
				unquoted = ft_strjoin_char(unquoted, (*input)[i]);
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
				unquoted = ft_strjoin_char(unquoted, (*input)[i]);
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
				unquoted = ft_strjoin_char(unquoted, (*input)[i]);
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
				if (flag || (quote.flag && i))
					unquoted = ft_strjoin_char(unquoted, (*input)[i]);
			}
			else if ((*input)[i] == '\\' || (*input)[i] == ';')
			{
				if (simp || ((*input)[i + 1]
					&& ((*input)[i + 1] == '(' || (*input)[i + 1] == ')')))
					unquoted = ft_strjoin_char(unquoted, (*input)[i]);
			}
			else
				unquoted = ft_strjoin_char(unquoted, (*input)[i]);
		}
		else
		{
			//aqui escribe las comillas escapadas o todo lo escapado
			unquoted = ft_strjoin_char(unquoted, (*input)[i]);
		}
		i++;
	}
	if (!unquoted)
		*input = ft_strdup("");
	else
		*input = unquoted;
	return ;
}
/*
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
	ft_printf("llega aqui\n");
	while ((*input)[i[0]])
	{
		if (!i[0] || (!(*input)[i[0] - 1] || (*input)[(i[0]) - 1] != '\\'))
			ft_unquote_first(&i, input, &unquoted, flag);
		else
			unquoted = ft_strjoin_char(unquoted, (*input)[(i[0])++]);
	}
	*input = unquoted;
	ft_printf("input apunta a : %s\n", *input);
}
*/