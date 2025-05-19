/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:44:53 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/17 20:08:10 by carlotalcd       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_count_brackets(char *str)
{
	int 	i;
	int	o_bracket;
	int	c_bracket;

	o_bracket = 0;
	c_bracket = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '(' && (!str[i - 1] || str[i - 1] != '\\'))
			o_bracket++;
		else if (str[i] == ')' && (!str[i - 1] || str[i - 1] != '\\'))
			c_bracket++;
		i++;
	}
	if (o_bracket == c_bracket)
		return (1);
	return (0);
}
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
		else if ((*input)[i] == '\\' && !simp && (!(*input)[i + 1] || ((*input)[i + 1] != '(' && (*input)[i + 1] != ')')))
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
int	ft_quit_brackets(t_token *token, int *open, int *close)
{
	int	to_quit;
	char *new_value;
	char *value;
	int	i;
	int	j;

	if (*open > *close)
		to_quit = *close;
	else
		to_quit = *open;
	*open -= to_quit;
	*close -= to_quit;
	value = token->str;
	i = 0;
	j = 0;
	/*
	new_value = (char *)malloc((ft_strlen(token->value) - to_quit + 1) * (char))
	if (!new_value)
		return (0);
		*/
	new_value = NULL;
	j = to_quit;
	while (value[i] == '(' && (!value[i - 1] || value[i - 1] != '\\') && j)
	{
		j--;
		i++;
	}
	j = to_quit;
	while (value[i] && (value[i] != ')' || (value[i - 1] && value[i - 1] == '\\')))
	{
		new_value = ft_strjoin_char(new_value, value[i]);
		if (!new_value)
			return (0);
		i++;
	}
	while (value[i] == ')' && (!value[i - 1] || value[i - 1] != '\\') && j)
	{
		j--;
		i++;
	}
	while (value[i])
	{
		new_value = ft_strjoin_char(new_value, value[i]);
		if (!new_value)
			return (0);
		i++;
	}
	ft_safe_free((void *)token->str);
	token->str = new_value;
	return (1);
}
//Esto bastante mal y quita las comillas de cada token, faltan las generales meterlas en token y las de comandos quitarlas
//Hay bucle infinito por ahi
int	ft_check_brackets(t_token *token, int *o_bracket, int *c_bracket)
{
	char	*value;
	int	i;
	//int	flag;
	int	open;
	int close;

	(void)o_bracket;
	(void)c_bracket;
	value = token->str;
	i = 0;
	open = 0;
	close = 0;
	while (value[i] && (value[i] == '(' && (!value[i - 1] || value[i - 1] != '\\')))
	{
		i++;
		open++;
	}
	while (value[i] && (value[i] != '(' && value[i] != ')'))
		i++;
	while (value[i] && (value[i] == ')' && (!value[i - 1] || value[i - 1] != '\\')))
	{
		i++;
		close++;
	}
	if (close && value[i])
		return (ft_printf("Syntax error near blabla \n"), 0);
	else if (open && close)
		ft_quit_brackets(token, &open, &close);
	return (1);
}


int	ft_manage_brackets(t_token *tokens)
{
	t_token *tmp;
	int o_bracket;
	int	c_bracket;

	tmp = tokens;
	o_bracket = 0;
	c_bracket = 0;
	while (tmp)
	{
		if (!ft_check_brackets(tmp, &o_bracket, &c_bracket))
			return (0);
		tmp = tmp->next;
	}
	return (1);
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
	if (!ft_count_brackets(src))
		return (ft_printf("Brackets not closed\n"), 0);
	s_input = ft_split_cmd(*input, ' '); 
	if (!s_input || !*s_input)
		return (0);
	//para eliminar los brackets, si tiene operador entre medias se deja, si no se quitan
	*input = ft_quit_quotes(s_input, minishell);
	if (!*input)
		return (0);
	return (1);
}
