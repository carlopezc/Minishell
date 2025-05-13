/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:44:53 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/13 16:27:13 by carlopez         ###   ########.fr       */
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

int	ft_check_word(char *input, t_minishell **minishell)
{
	t_quote	quote;
	int	i;
	//char	*value;
	//char	*tmp;

	quote.type = 0;
	quote.flag = 0;
	i = 0;
	(void)minishell;
	//value = NULL;
	//tmp = NULL;
	while (input[i])
	{

		if (!input[i - 1] || input[i - 1] != '\\')
			ft_check_quote(&quote, input[i], &i);
		if (quote.flag && !ft_strchr(&input[i], ' '))
			return (0);
		i++;
		//aqui igual habria que anadir aparte de espacios tabulaciones y blabla
		//voy a intentar expandir aqui variables
		/*
		if (input[i] == '$' && quote.type != '\'')
		{
			tmp = ft_check_var(*minishell, input, &i);
			if (tmp)
			{
				value = ft_strjoin(value, tmp);
				free(tmp);
			}
		}
		*/
		//ahora veo que return hago
	}
	return (1);
}

int	ft_strlen_quoted(char *input)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	ft_printf("Input: %s\n", input);
	while (input[i])
	{
		ft_printf("Char es: %c\n", input[i]);
		if (input[i] == '\\')
			i++;
		else if (input[i] == '\'' || input[i] == '\"')
		{
			if (input[i - 1] && input[i - 1] == '\\')
				len++;
			else if (ft_strchr(&input[i], ' '))
				len += 2;
			i++;
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

void	ft_unquote(char **input)
{
	char *unquoted;
	int	i;
	int	j;
	int	len;
	int	in_word;

	i = 0;
	j = 0;
	in_word = 0;
	len = ft_strlen_quoted(*input);
	ft_printf("UNQUOTE\n");
	ft_printf("len es %d\n", len);
	unquoted = (char *)malloc((len + 1) * sizeof(char));
	if (!unquoted)
		return ;
	while ((*input)[i])
	{
		if (!(*input)[i - 1] || (*input)[i - 1] != '\\')
		{
			if (((*input)[i] == '\'' || (*input)[i] == '\"') && in_word)
			{
				ft_printf("Entra en 1\n");
				in_word = 0;
				unquoted[j++] = (*input)[i++];
			}
			if (((*input)[i] == '\"') && ((ft_strchr(&(*input)[i], ' ')) && (ft_strchr(&(*input)[i + 1], '\"')) && (ft_strchr(&(*input)[i], ' ') < ft_strchr(&(*input)[i + 1], '\"'))))
			{
				ft_printf("Entra en 2\n");
				unquoted[j++] = (*input)[i++];
				in_word = 1;
			}
			if (((*input)[i] == '\'') && ((ft_strchr(&(*input)[i], ' ')) && (ft_strchr(&(*input)[i + 1], '\'')) && (ft_strchr(&(*input)[i], ' ') < ft_strchr(&(*input)[i + 1], '\''))))
			{
				ft_printf("Entra en 3\n");
				unquoted[j++] = (*input)[i++];
				in_word = 1;
			}
			else if ((*input)[i] == '\"' || (*input)[i] == '\'')
			{
				ft_printf("Entra en 4\n");
				i++;
			}
			else if ((*input)[i] == '\\')
				i++;
			else
			{
				ft_printf("Entra en 5\n");
				unquoted[j++] = (*input)[i++];
			}
		}
		else if ((*input)[i] == '\\')
			i++;
		else
		{
			ft_printf("Entra en 6\n");
			unquoted[j++] = (*input)[i++];
		}
	}
	unquoted[j] = '\0';
	*input = unquoted;
	ft_printf("Input unquoted: %s\n", *input);
	ft_printf("Hecho\n");
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

	quote.flag = 0;
	quote.type = 0;
	i = 0;
	if (!*input || !(*input)[0])
		return ;
	dollar = ft_strchr(*input, '$');
	if (!dollar)
		return ;
	while ((*input)[i])
	{
		if ((*input)[i - 1] && (*input)[i - 1] != '\\')
			ft_check_quote(&quote, (*input)[i], &i);
 		if ((*input)[i] == '$' && quote.type != '\'')
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
					*input = ft_expand(*input, &i, (*minishell)->env);
					return ;
				}
			}
                }
		i++;
	}

}

char	*ft_quit_quotes(char **s_input, t_minishell **minishell)
{
	int	i;
	char *input;

	i = 0;
	ft_printf("El split de comillas : \n");
	(void)minishell;
	while (s_input[i])
	{
		ft_printf("%s\n", s_input[i]);
		ft_printf("Ha entrado\n");
		ft_variable(&s_input[i], minishell);
		ft_unquote(&s_input[i]);
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
	if (!ft_count_quotes(src))
		return (ft_printf("Quotes not closed\n"), 0);
	s_input = ft_split_cmd(*input, ' ');
	if (!s_input || !*s_input)
		return (0);
	*input = ft_quit_quotes(s_input, minishell);
	if (!*input)
		return (0);
	return (1);
}
