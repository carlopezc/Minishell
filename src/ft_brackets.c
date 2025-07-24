/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_brackets.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:26:24 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/24 01:26:35 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_set_to_quit(int *open, int *close, int *to_quit)
{
	*to_quit = *open;
	if (*open > *close)
		*to_quit = *close;
	*open -= *to_quit;
	*close -= *to_quit;
	return ;
}

int	ft_add_while(char *value, int *i, char c, char **new_value)
{
	if (c == 0)
	{
		while (value[*i])
		{
			*new_value = ft_strjoin_char(*new_value, value[*i]);
			if (!(*new_value))
				return (0);
			(*i)++;
		}
		return (1);
	}
	while (value[*i] && (value[*i] != c
			|| (value[*i - 1] && value[*i - 1] == '\\')))
	{
		*new_value = ft_strjoin_char(*new_value, value[*i]);
		if (!(*new_value))
			return (0);
		(*i)++;
	}
	return (1);
}

int	ft_quit_brackets(t_token *token, int *open, int *close)
{
	int		to_quit;
	char	*new_value;
	char	*value;
	int		i;
	int		j;

	ft_set_to_quit(open, close, &to_quit);
	value = token->str;
	i = 0;
	new_value = NULL;
	j = to_quit;
	while (value[i] && value[i] == '(' && (!i 
			|| value[i - 1] != '\\') && (j-- > 0))
		i++;
	if (!ft_add_while(value, &i, ')', &new_value))
		return (0);
	j = to_quit;
	while (value[i] == ')' && (!i 
			|| value[i - 1] != '\\') && (j-- > 0))
		i++;
	if (!ft_add_while(value, &i, 0, &new_value))
		return (0);
	ft_safe_free((void **)&(token->str));
	token->str = new_value;
	return (1);
}

int	ft_quit_brckt_dup(t_token *tmp, char c)
{
	int		i;
	char	*str;
	char	*new_str;

	i = 0;
	str = tmp->str;
	new_str = NULL;
	if (c == '(')
	{
		while (str[i + 1] && str[i + 1] == c)
			i++;
		new_str = ft_substr(str, i, ft_strlen(str) - i);
		ft_safe_free((void **)&(tmp->str));
		tmp->str = new_str;
	}
	else if (c == ')')
	{
		while (str[i] && (str[i] != c || (!i || str[i - 1] == '\\')))
			i++;
		if (str[i] && str[i] == c)
		{
			new_str = ft_substr(str, 0, i + 1);
			ft_safe_free((void **)&(tmp->str));
			tmp->str = new_str;
		}
	}
	if (!tmp->str)
		return (0);
	return (1);
}

int	ft_manage_brackets(t_token *tokens)
{
	t_token	*tmp;

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
