/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:34:23 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/21 19:39:40 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

char	*ft_expand(char *input, int *i, t_env *env)
{
	char	*name_var;
	t_env	*tmp;
	int		start;

	if (!input || !env)
		return (NULL);
	start = *i;
	if (!ft_isalpha(input[*i]) && (input[*i] != '_'))
		return (ft_printf("Non valid name\n"), ft_strdup(""));
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	if (*i == start)
		return (ft_strdup("$"));
	name_var = ft_substr(input, start, *i - start);
	if (!name_var)
		return (NULL);
	tmp = env;
	while (tmp)
	{
		if (!ft_strncmp(name_var,
				tmp->name, ft_max_strlen(tmp->name, name_var)))
			return (ft_safe_free((void **)&name_var), ft_strdup(tmp->value));
		tmp = tmp->next;
	}
	return (ft_safe_free((void **)&name_var), NULL);
}
