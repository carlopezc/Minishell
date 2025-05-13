/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:44:53 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/13 19:17:25 by carlopez         ###   ########.fr       */
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
/*
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
	
		if (input[i] == '$' && quote.type != '\'')
		{
			tmp = ft_check_var(*minishell, input, &i);
			if (tmp)
			{
				value = ft_strjoin(value, tmp);
				free(tmp);
			}
		}
		
		//ahora veo que return hago
	}
	return (1);
}
*/
int	ft_strlen_quoted(char *input)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (input[i])
	{
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
	unquoted = (char *)malloc((len + 1) * sizeof(char));
	if (!unquoted)
		return ;
	while ((*input)[i])
	{
		if (!(*input)[i - 1] || (*input)[i - 1] != '\\')
		{
			if (((*input)[i] == '\'' || (*input)[i] == '\"') && in_word)
			{
				in_word = 0;
				unquoted[j++] = (*input)[i++];
			}
			if (((*input)[i] == '\"') && ((ft_strchr(&(*input)[i], ' ')) && (ft_strchr(&(*input)[i + 1], '\"')) && (ft_strchr(&(*input)[i], ' ') < ft_strchr(&(*input)[i + 1], '\"'))))
			{
				unquoted[j++] = (*input)[i++];
				in_word = 1;
			}
			if (((*input)[i] == '\'') && ((ft_strchr(&(*input)[i], ' ')) && (ft_strchr(&(*input)[i + 1], '\'')) && (ft_strchr(&(*input)[i], ' ') < ft_strchr(&(*input)[i + 1], '\''))))
			{
				unquoted[j++] = (*input)[i++];
				in_word = 1;
			}
			else if ((*input)[i] == '\"' || (*input)[i] == '\'')
				i++;
			else if ((*input)[i] == '\\')
				i++;
			else
				unquoted[j++] = (*input)[i++];
		}
		else if ((*input)[i] == '\\')
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
	ft_printf("Entra en reemplazar variable\n");
	dollar = ft_strchr(*input, '$');
	if (!dollar)
		return ;
	while ((*input)[i])
	{
		if ((*input)[i - 1] && (*input)[i - 1] != '\\')
			ft_check_quote(&quote, (*input)[i], &i);
 		if ((*input)[i] == '$' && quote.type != '\'')
                {
			//substr hasta el dollar luego remplazo y lo de despues
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
					ft_printf("Final es %s\n", final);
				}
			}
			else
				final = ft_strjoin(final, ft_expand(*input, &i, (*minishell)->env));
                }
		else
		{
			final = ft_strjoin_char(final, (*input)[i]);
			ft_printf("Final es %s\n", final);
		}
		i++;
	}
	*input = final;
	ft_printf("El input que devuelve reemplazar es : %s\n", *input);
	return ;
}

char	*ft_quit_quotes(char **s_input, t_minishell **minishell)
{
	int	i;
	char *input;

	i = 0;
	//ft_printf("El split de comillas : \n");
	while (s_input[i])
	{
		//ft_printf("%s\n", s_input[i]);
		//ft_printf("Ha entrado\n");
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
