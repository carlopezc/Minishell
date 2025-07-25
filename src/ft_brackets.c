/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_brackets.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:26:24 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/25 09:48:25 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

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

static int	ft_trim_open_paren(t_token *tmp, char *str)
{
	int		i;
	char	*new_str;

	i = 0;
	while (str[i + 1] && str[i + 1] == '(')
		i++;
	new_str = ft_substr(str, i, ft_strlen(str) - i);
	if (!new_str)
		return (0);
	ft_safe_free((void **)&(tmp->str));
	tmp->str = new_str;
	return (1);
}

static int	ft_trim_close_paren(t_token *tmp, char *str)
{
	int		i;
	char	*new_str;

	i = 0;
	new_str = NULL;
	while (str[i] && (str[i] != ')' || (!i || str[i - 1] == '\\')))
		i++;
	if (str[i] && str[i] == ')')
	{
		new_str = ft_substr(str, 0, i + 1);
		if (!new_str)
			return (0);
	}
	ft_safe_free((void **)&(tmp->str));
	tmp->str = new_str;
	return (1);
}

int	ft_quit_brckt_dup(t_token *tmp, char c)
{
	if (!tmp || !tmp->str)
		return (0);
	if (c == '(')
		return (ft_trim_open_paren(tmp, tmp->str));
	else if (c == ')')
		return (ft_trim_close_paren(tmp, tmp->str));
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
