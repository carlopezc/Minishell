/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcard.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:57:16 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/14 16:18:54 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

char	*ft_expand_wildcard(char *input, t_token_type type)
{
	char	**elements;

	elements = ft_get_elements();
	if (!elements)
		return (ft_free_array(elements), NULL);
	if (!ft_strncmp(input, "*", 2))
		return (ft_create_array(elements));
	if (!ft_check_asterisk(input, &elements))
		return (NULL);
	if ((type == REDIR_IN || type == REDIR_OUT) && (ft_arraylen(elements) > 1))
		return (ft_printf("Error in files to redirect\n"), NULL);
	if (!ft_arraylen(elements))
		return (ft_strdup(input));
	return (ft_create_array(elements));
}

int	ft_get_pattern(char *str, int *i, char **str_final, t_token **tmp)
{
	int		start;
	char	*sub;
	char	*expanded;

	start = *i;
	sub = NULL;
	expanded = NULL;
	while (start > 0 && (str[start - 1]
			&& str[start - 1] != ' ' && start > 0))
		start--;
	(*i)++;
	while (str[*i] && str[*i] != ' ')
		(*i)++;
	sub = ft_substr(str, start, *i - start);
	expanded = ft_expand_wildcard(sub, (*tmp)->type);
	if (!expanded)
		return (0);
	if (str[*i] && str[*i] == ' ')
		expanded = ft_strjoin(expanded, " ");
	*str_final = ft_strjoin(*str_final, expanded);
	return (free(sub), free(expanded), 1);
}

int	ft_wildcard_loop(char *str, char **str_final, t_token **tmp)
{
	int		i;
	t_quote	quote;
	int	flag;

	i = 0;
	ft_init_quote(&quote);
	while (str[i])
	{
		flag = 0;
		if (!i || str[i - 1] != '\\')
		{
			ft_check_quote(&quote, str[i]);
			if (quote.flag && (str[i] == '\'' || str[i] == '\"'))
				i++;
		}
		if (str[i] == '\\' && str[i + 1] == '*' && !quote.flag)
		{
			//si entra aqui se come un caracter al final
			*str_final = ft_strjoin_char(*str_final, str[i + 1]);
			i += 2;
			flag = 1;
		}
		else if (str[i] && str[i] == '*' && !quote.flag
			&& (!i || str[i - 1] != '\\'))
		{
			if (!ft_get_pattern(str, &i, str_final, tmp))
				return (0);
		}
		else if ((str[i] == '\'' || str[i] == '\"') && str[i - 1] == '\\' && !ft_find_asterisk(&str[i]))
			*str_final = ft_strjoin_char(*str_final, str[i]);
		else if (!quote.flag && ((str[i] == '*' && ft_find_asterisk(&str[i]))
				|| (!ft_is_quote(str[i]) && !ft_find_asterisk(&str[i]))))
			*str_final = ft_strjoin_char(*str_final, str[i]);
		else if (quote.flag && str[i] != '\'' && str[i] != '\"')
			*str_final = ft_strjoin_char(*str_final, str[i]);
		if (str[i] && !flag)
			i++;
	}
	return (1);
}

int	ft_check_wildcard(t_token **tokens)
{
	t_token	*tmp;
	char	*str;
	char	*str_final;
	char	*asterisk;

	tmp = *tokens;
	if (!tokens || !*tokens)
		return (1);
	str_final = NULL;
	while (tmp)
	{
		asterisk = ft_strchr(tmp->str, '*');
		if (asterisk)
		{
			str = tmp->str;
			if (!ft_parse_asterisk(&str))
				return (0);
			if (!ft_wildcard_loop(str, &str_final, &tmp))
				return (0);
			tmp->str = ft_strdup(str_final);
			ft_safe_free((void **)&str_final);
		}
		tmp = tmp->next;
	}
	return (1);
}
