/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:44:53 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/28 21:08:17 by carlopez         ###   ########.fr       */
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
		if (str[i] == '(' /*&& (!str[i - 1] || str[i - 1] != '\\')*/)
		{
			/*
			if (str[i + 1] && str[i + 1] == '<')
				return (ft_printf("parse error near blabla\n"), 0);
				*/
			o_bracket++;
		}
		else if (str[i] == ')'/* && (!str[i - 1] || str[i - 1] != '\\')*/)
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
/*
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
*/
void	ft_unquote(char **input, int flag)
{
	char *unquoted;
	int	i;
//	int	j;
	//int	len;
	int	in_word;
	int	simp;
	int	asterisk;

	i = 0;
//	j = 0;
	in_word = 0;
	simp = 0;
	asterisk = 0;
	//len = ft_strlen_quoted(*input, flag);
	//unquoted = (char *)malloc((len + 1) * sizeof(char));
	//if (!unquoted)
		//return ;
	unquoted = NULL;
	while ((*input)[i])
	{
		if (i > 0 && (!(*input)[i - 1] || (*input)[i - 1] != '\\'))
		{
			if (((*input)[i] == '\'' || (*input)[i] == '\"') && (flag || asterisk) && in_word)
			{
				in_word = 0;
				unquoted = ft_strjoin_char(unquoted, (*input)[i++]);
			}
			else if (((*input)[i] == '\"') && flag && ((ft_strchr(&(*input)[i], ' ')) && (ft_strchr(&(*input)[i + 1], '\"')) && (ft_strchr(&(*input)[i], ' ') < ft_strchr(&(*input)[i + 1], '\"'))))
			{
				in_word = 1;
				unquoted = ft_strjoin_char(unquoted, (*input)[i++]);
			}
			else if (((*input)[i] == '\"') && ((ft_strchr(&(*input)[i], '*')) && (ft_strchr(&(*input)[i + 1], '\"')) && (ft_strchr(&(*input)[i], '*') < ft_strchr(&(*input)[i + 1], '\"'))))
			{
				if (!asterisk)
					asterisk = 1;
				else
					asterisk = 0;
				in_word = 1;
				unquoted = ft_strjoin_char(unquoted, (*input)[i++]);
			}
			else if (((*input)[i] == '\'') && ((ft_strchr(&(*input)[i], '*')) && (ft_strchr(&(*input)[i + 1], '\'')) && (ft_strchr(&(*input)[i], '*') < ft_strchr(&(*input)[i + 1], '\''))))
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
			else if (((*input)[i] == '\'') && flag && ((ft_strchr(&(*input)[i], ' ')) && (ft_strchr(&(*input)[i + 1], '\'')) && (ft_strchr(&(*input)[i], ' ') < ft_strchr(&(*input)[i + 1], '\''))))
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
				if (simp || ((*input)[i + 1] && ((*input)[i + 1] == '(' || (*input)[i + 1] == ')')))
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
	unquoted = ft_strjoin_char(unquoted, '\0');
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
		if (*s_input[i])
		{
			tmp = ft_strjoin(" ", s_input[i]);
			input = ft_strjoin(input, tmp);
			free(tmp);
			i++;
		}
		else
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
					final = ft_strjoin(final, ft_expand(*input, &i, (*minishell)->env));
			}
			else
			{
				final = ft_strjoin_char(final, (*input)[i]);
				if ((*input)[i + 1] && ((*input)[i + 1] != '\'' && (*input)[i + 1] != '\"'))
					i--;
			}
        	}
		else
			final = ft_strjoin_char(final, (*input)[i++]);
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
		//en unquote dejo las comillas en los asteriscos y luego en la wildcard los quito, ademas que en los export y env que su valor tenga espacio tambien los tengo que dejar
		ft_unquote(&s_input[i], flag);
		if (!ft_strncmp(s_input[i], "export", 7) || !ft_strncmp(s_input[i], "env", 4))
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
	token->str = new_value;
	return (1);
}

int	ft_check_brackets(t_token *token)
{
	char	*value;
	int	i;
	int	open;
	int close;

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
	while (value[i] && value[i] == ' ')
		i++;
	if (close && value[i])
		return (ft_printf("parse error near blabla \n"), 0);
	else if (open && close)
		ft_quit_brackets(token, &open, &close);
	return (1);
}

int	ft_quit_brckt_dup(t_token *tmp, char c)
{
	int	i;
	char *str;

	i = 0;
	str = tmp->str;
	if (c == '(')
	{
		while (str[i + 1] && str[i + 1] == c)
			i++;
		str = ft_substr(str, i, ft_strlen(str) - i);
		tmp->str = str;
	}
	else if (c == ')')
	{
		while (str[i] && (str[i] != c || (str[i - 1] && str[i - 1] == '\\')))
			i++;
		if (str[i] && str[i] == c)
			str = ft_substr(str, 0, i + 1);
		tmp->str = str;
	}
	if (!str)
		return (0);
	return (1);
}

int	ft_check_next(t_token *tokens, int o_brckt)
{
	t_token	*tmp;
	int	c_brckt;
	int	i;

	tmp = tokens;
	c_brckt = 0;
	i = 0;
	while (tmp)
	{
		while ((tmp->str)[i])
		{
			if ((tmp->str)[i] && (tmp->str)[i] == ')' && ((!(tmp->str)[i - 1] || (tmp->str)[i - 1] != '\\')))
				c_brckt++;
			i++;
		}
		if (c_brckt == o_brckt)
		{
			if (!ft_quit_brckt_dup(tmp, ')'))
				return (0);
			return (2);
		}
		c_brckt = 0;
		i = 0;
		tmp = tmp->next;
	}
	return (1);
}

int	ft_last_check(t_token *tokens)
{
	t_token *tmp;
	int o_brckt;
	int	i;
	int	res;

	tmp = tokens;
	o_brckt = 0;
	i = 0;
	res = 0;
	while (tmp)
	{
		while ((tmp->str)[i])
		{
			if ((tmp->str)[i] && (tmp->str)[i] == '(' && ((!(tmp->str)[i - 1] || (tmp->str)[i - 1] != '\\')))
				o_brckt++;
			i++;
		}
		if (o_brckt > 1)
		{
			res = ft_check_next(tokens, o_brckt);
			if (res == 0)
				return (0);
			else if (res == 2)
			{
				if (!ft_quit_brckt_dup(tmp, '('))
					return (0);
			}
		}
		o_brckt = 0;
		i = 0;
		tmp = tmp->next;
	}
	return (1);
}

int	ft_manage_brackets(t_token *tokens)
{
	t_token *tmp;

	tmp = tokens;
	while (tmp)
	{
		if (!ft_check_brackets(tmp))
			return (0);
		tmp = tmp->next;
	}
	if (!ft_last_check(tokens))
		return (0);
	return (1);
}

void	ft_quit_first_last(t_token **token)
{
	t_token	*tmp;
	t_token	*next;
	t_token *prev;

	next = *token;
	*token = next->next;
	free(next->str);
	free(next);
	tmp = *token;
	while (tmp && tmp->next)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	free(tmp->str);
	free(tmp);
	prev->next = NULL;
	return ;
	//no se si puedo borrar el type
}

void	ft_last(t_token **token)
{
	t_token	*tmp;
	int	o_bracket;
	int	c_bracket;

	tmp = *token;
	o_bracket = 0;
	c_bracket = 0;
	if (tmp->type != O_BRACKET)
		return ;
	tmp = tmp->next;
	while (tmp && tmp->next)
	{
		if (tmp->type == O_BRACKET)
			o_bracket++;
		else if (tmp->type == C_BRACKET)
			c_bracket++;
		tmp = tmp->next;
	}
	if (o_bracket != c_bracket)
		return ;
	if (tmp->type == C_BRACKET)
		ft_quit_first_last(token);
	return ;
}

int	ft_add_bracket_token(t_token **token)
{
	t_token	*tmp;
	char	*str;
	int	i;
	int	start;
	char	*cpy;
	t_token *new_token;
	t_token *prev;

	if (!ft_manage_brackets(*token))
		return (0);
	tmp = *token;
	if (!tmp)
		return (1);
	str = tmp->str;
	i = 0;
	start = 0;
	cpy = NULL;
	prev = NULL;
	//hacerlo anadiendo el de antes
	while (tmp)
	{
		str = tmp->str;
		while (str[i])
		{
			if (str[i] == '(' && (!str[i - 1] || str[i - 1] != '\\'))
			{
				start = i;
				new_token = ft_create_token("(", O_BRACKET);
				if (!new_token)
					return (0);
				ft_connect_token(token, new_token, prev);
				tmp->str = ft_substr(str, start + 1, ft_strlen(str) - (start + 1));
				str = tmp->str;
				i--;
			}
			else if (str[i] == ')' && (!str[i - 1] || str[i - 1] != '\\'))
			{
				cpy = ft_strdup(str);
				tmp->str = ft_substr(str, 0, i);
				while (tmp && cpy[i] && cpy[i] == ')' && (!cpy[i - 1] || cpy[i] != '\\'))
				{
					new_token = ft_create_token(")", C_BRACKET);
					if (!new_token)
						return (0);
					ft_connect_token(token, new_token, tmp);
					tmp = tmp->next;
					i++;
				}
				if (!cpy[i])
					break ;
			}
			i++;
		}
		prev = tmp;
		tmp = tmp->next;
		start = 0;
		i = 0;
	}
	ft_last(token);
	return (1);
}

int	ft_parsing(char **input, t_minishell **minishell)
{
	char	*src;
	char	**s_input;

	if (!*input | !(*input)[0])
		return (1);
	src = *input;
	if (!ft_count_quotes(src))
		return (ft_printf("Quotes not closed\n"), 0);
	if (!ft_count_brackets(src))
		return (ft_printf("Brackets not closed\n"), 0);
	s_input = ft_split_cmd(*input, ' '); 
	if (!s_input || !*s_input)
		return (0);
	*input = ft_quit_quotes(s_input, minishell);
	if (!*input)
		return (0);
	return (1);
}
