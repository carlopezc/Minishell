/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:51:45 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/30 11:45:32 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

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
