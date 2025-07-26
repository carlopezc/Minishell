/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:51:45 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/26 01:38:02 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

void	ft_print_value(char *value)
{
	int	i;

	i = 0;
	ft_printf("\"");
	while (value[i])
	{
		if (value[i] == '\"')
			write(1, "\\", 1);
		if (value[i] == '$')
			write(1, "\\", 1);
		if (value[i] == '\\')
			write(1, "\\", 1);
		write(1, &value[i], 1);
		i++;
	}
	ft_printf("\"\n");
	return ;
}

void	ft_print_export(t_minishell *shell)
{
	t_env	*tmp;

	ft_sort_list(shell->export);
	tmp = shell->export;
	while (tmp)
	{
		if (!tmp->value)
			ft_printf("declare -x %s\n", tmp->name);
		else if (tmp->value[0] == '\0')
			ft_printf("declare -x %s=\"\"\n", tmp->name);
		else
		{
			if (!(tmp->name[0] == '_' && tmp->name[1] == '\0'))
			{
				ft_printf("declare -x %s=", tmp->name);
				ft_print_value(tmp->value);
			}
		}
		tmp = tmp->next;
	}
	return ;
}

void	ft_print_elements(char **elements)
{
	int	i;

	i = 0;
	while (elements[i])
		ft_printf("%s ", elements[i++]);
	return ;
}

void	ft_print_tokens(t_token	*token)
{
	t_token	*tmp;

	tmp = token;
	while (tmp)
	{
		ft_printf("El input es %s\n", tmp->str);
		ft_printf("El token type es: %s\n", token_to_str(tmp->type));
		tmp = tmp->next;
	}
	return ;
}

void	ft_print_array(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
		ft_printf("%s\n", arr[i++]);
	return ;
}
