/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:44:53 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/15 18:35:00 by carlotalcd       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_count_quotes(char *str)
{
	int	i;
	int	double_quotes;
	int	simple_quotes;

	i = 0;
	simple_quotes = 0;
	double_quotes = 0;
	while (str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
		{
			if (!str[i - 1] || (str[i - 1] && str[i - 1] != '\\'))
			{
				if (str[i] == '\"')
					double_quotes++;
				else
					simple_quotes++;
			}
		}
		i++;
	}
	if (!(double_quotes % 2) && !(simple_quotes % 2))
		return (1);
	return (0);
}

int	ft_strlen_quoted(char *input, int flag)
{
	int	i;
	int	len;
	int	simp;

	i = 0;
	len = 0;
	simp = 0;
	while (input[i])
	{
		if (input[i] == '\\' && !simp)
			i++;
		else if (input[i] == '\'' || input[i] == '\"')
		{
			if (input[i] == '\'' && (!input[i - 1] || input[i - 1] != '\\'))
			{
				if (simp)
					simp = 0;
				else
					simp = 1;
			}
			if (input[i - 1] && input[i - 1] == '\\')
				len++;
			else if (ft_strchr(&input[i], ' ') && flag)
				len += 2;
			i++;
		}
		else if (input[i] == ';' && !simp && (!input[i - 1] || input[i - 1] != '\\'))
			i++;
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

void	ft_unquote(char **input, int flag)
{
	char *unquoted;
	int	i;
	int	j;
	int	len;
	int	in_word;
	int	simp;

	i = 0;
	j = 0;
	in_word = 0;
	simp = 0;
	len = ft_strlen_quoted(*input, flag);
	unquoted = (char *)malloc((len + 1) * sizeof(char));
	if (!unquoted)
		return ;
	while ((*input)[i])
	{
		if (!(*input)[i - 1] || (*input)[i - 1] != '\\')
		{
			if (((*input)[i] == '\'' || (*input)[i] == '\"') && flag && in_word)
			{
				in_word = 0;
				unquoted[j++] = (*input)[i++];
			}
			else if (((*input)[i] == '\"') && flag && ((ft_strchr(&(*input)[i], ' ')) && (ft_strchr(&(*input)[i + 1], '\"')) && (ft_strchr(&(*input)[i], ' ') < ft_strchr(&(*input)[i + 1], '\"'))))
			{
				unquoted[j++] = (*input)[i++];
				in_word = 1;
			}
			else if (((*input)[i] == '\'') && flag && ((ft_strchr(&(*input)[i], ' ')) && (ft_strchr(&(*input)[i + 1], '\'')) && (ft_strchr(&(*input)[i], ' ') < ft_strchr(&(*input)[i + 1], '\''))))
			{
				unquoted[j++] = (*input)[i++];
				in_word = 1;
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
				if (simp)
					unquoted[j++] = (*input)[i++];
				else
					i++;
			}
			else
				unquoted[j++] = (*input)[i++];
		}
		else if ((*input)[i] == '\\' && !simp)
			i++;
		else
			unquoted[j++] = (*input)[i++];
	}
	unquoted[j] = '\0';
	*input = unquoted;
	return ;
}

char	*ft_create_array(char **s_input)
{
	char	*input;
	char	*tmp;
	int	i;

	i = 0;
	input = NULL;
	tmp = NULL;
	if (!s_input || !*s_input)
		return (NULL);
	input = ft_strdup(s_input[i]);
	i++;
	while (s_input[i])
	{
		tmp = ft_strjoin(" ", s_input[i]);
		input = ft_strjoin(input, tmp);
		free(tmp);
		i++;
	}
	return (input);
}

void	ft_variable(char **input, t_minishell **minishell)
{
	char *dollar;
	t_quote quote;
	int	i;
	char	*final;

	quote.flag = 0;
	quote.type = 0;
	i = 0;
	final = NULL;
	if (!*input || !(*input)[0])
		return ;
	dollar = ft_strchr(*input, '$');
	if (!dollar)
		return ;
	while ((*input)[i])
	{
		if (!(*input)[i - 1] || ((*input)[i - 1] && (*input)[i - 1] != '\\'))
			ft_check_quote(&quote, (*input)[i], &i);
 		if ((*input)[i] == '$' && (quote.type != '\'' && (!(*input)[i - 1] || ((*input)[i - 1] && (*input)[i - 1] != '\\'))))
        {
			if ((!(*input)[i - 1]) || ((*input)[i - 1] && (*input)[i - 1] != '\\'))
			{
				i++;
				if ((*input)[i] == '?')
				{
					ft_printf("Aqui ira el codigo de salida\n");
					return ;
				}
				else
				{
					final = ft_strjoin(final, ft_expand(*input, &i, (*minishell)->env));
					if ((*input)[i + 1] && ((*input)[i + 1] != '\'' && (*input)[i + 1] != '\"'))
						i--;
				}
			}
			else
			{
				final = ft_strjoin_char(final, (*input)[i]);
				if ((*input)[i + 1] && ((*input)[i + 1] != '\'' && (*input)[i + 1] != '\"'))
					i--;
			}
        }
		else
			final = ft_strjoin_char(final, (*input)[i]);
		/*
		if ((*input)[i] == '\\')
			final = ft_strjoin_char(final, '\\');
			*/
		i++;
	}
	*input = final;
	return ;
}

char	*ft_quit_quotes(char **s_input, t_minishell **minishell)
{
	int	i;
	char *input;
	int	flag;

	i = 0;
	flag = 0;
	while (s_input[i])
	{
		ft_variable(&s_input[i], minishell);
		ft_unquote(&s_input[i], flag);
		if (!ft_strncmp(s_input[i], "export", 7))
			flag = 1;
		if (ft_check_operator(s_input[i]))
			flag = 0;
		i++;
	}
	input = ft_create_array(s_input);
	return (input);
}

int	ft_parsing(char **input, t_minishell **minishell)
{
	char	*src;
	char	**s_input;

	if (!input || !*input)
		return (0);
	src = *input;
	//aqui miro quotes y brackets
	if (!ft_count_quotes(src))
		return (ft_printf("Quotes not closed\n"), 0);
	s_input = ft_split_cmd(*input, ' ');
	if (!s_input || !*s_input)
		return (0);
	//para eliminar los brackets, si tiene operador entre medias se deja, si no se quitan
	*input = ft_quit_quotes(s_input, minishell);
	if (!*input)
		return (0);
	return (1);
}
