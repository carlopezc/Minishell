/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcard.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:57:16 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/24 16:13:39 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_find_asterisk_in_word(const char *str, int index)
{
	int	start;
	int	end;
	int	i;

	start = index;
	end = index;
	while (start > 0 && str[start - 1] != ' ')
		start--;
	while (str[end] && str[end] != ' ')
		end++;
	i = start;
	while (i < end)
	{
		if (str[i] == '*')
			return (1);
		i++;
	}
	return (0);
}

char	*ft_expand_wildcard(char *input, t_token_type type)
{
	char	**elements;
	char	*elements_input;

	elements = ft_get_elements();
	if (!elements)
		return (ft_free_array(elements), NULL);
	if (!ft_strncmp(input, "*", 2))
	{
		elements_input = ft_create_array(elements);
		return (ft_free_array(elements), elements_input);
	}
	if (!ft_check_asterisk(input, &elements))
		return (NULL);
	if ((type == REDIR_IN || type == REDIR_OUT) && (ft_arraylen(elements) > 1))
		return (ft_free_array(elements),
			ft_printf("Error in files to redirect\n"), NULL);
	if (!ft_arraylen(elements))
		return (ft_free_array(elements), ft_strdup(input));
	elements_input = ft_create_array(elements);
	return (ft_free_array(elements), elements_input);
}

int	ft_get_pattern(char *str, int *i, char **str_final, t_token **tmp)
{
	int		start;
	char	*sub;
	char	*expanded;
	char	*str_tmp;
	char	*str_tmp2;

	start = *i;
	sub = NULL;
	expanded = NULL;
	str_tmp = NULL;
	str_tmp2 = NULL;
	while (start > 0 && (str[start - 1]
			&& str[start - 1] != ' ' && start > 0))
		start--;
	(*i)++;
	while (str[*i] && str[*i] != ' ')
		(*i)++;
	sub = ft_substr(str, start, *i - start);
	expanded = ft_expand_wildcard(sub, (*tmp)->type);
	if (!expanded)
		return (free(sub), 0);
	if (str[*i] && str[*i] == ' ')
	{
		str_tmp2 = expanded;
		expanded = ft_strjoin(expanded, " ");
		ft_safe_free((void **)&str_tmp2);
	}
	str_tmp = ft_strdup(*str_final);
	ft_safe_free((void **)str_final);
	*str_final = ft_strjoin(str_tmp, expanded);
	return (free(sub), free(expanded), free(str_tmp), 1);
}

int	ft_wildcard_loop(char *str, char **str_final, t_token **tmp)
{
	int		i;
	t_quote	quote;
	int		flag;
	int		s_flag;

	ft_init_quote(&quote);
	i = 0;
	s_flag = 0;
	while (str[i])
	{
		flag = 0;
		ft_handle_quotes_and_skip(str, &i, &quote, &s_flag);
		if (ft_handle_backslash_quotes(str, i, &quote, str_final)
			|| ft_handle_escaped_asterisk(str, &i, &flag, str_final));
		else if (ft_is_real_asterisk(str, i, &quote))
		{
			if (!ft_process_asterisk_pattern(str, &i, str_final, tmp))
				return (0);
		}
		else if (ft_handle_quotes_in_word(str, i, str_final)
			|| ft_handle_normal_chars(str, i, &quote, str_final))
			;
		if (str[i] && !flag)
			i++;
	}
	return (1);
}

int	ft_check_wildcard(t_token **tokens)
{
	t_token	*tmp;
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
			if (!ft_parse_asterisk(&(tmp->str)))
				return (0);
			if (!ft_wildcard_loop(tmp->str, &str_final, &tmp))
				return (0);
			ft_safe_free((void **)&(tmp->str));
			tmp->str = ft_strdup(str_final);
			ft_safe_free((void **)&(str_final));
		}
		tmp = tmp->next;
	}
	return (1);
}
