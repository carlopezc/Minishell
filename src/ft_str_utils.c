/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 19:45:39 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/30 19:46:42 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

char	*ft_strjoin_char(char *str, char c)
{
	int		i;
	char	*final;

	i = 0;
	if (str)
		i = ft_strlen(str);
	final = (char *)malloc((i + 2) * sizeof(char));
	if (!final)
		return (NULL);
	i = 0;
	while (str && str[i])
	{
		final[i] = str[i];
		i++;
	}
	final[i++] = c;
	final[i] = '\0';
	ft_safe_free((void **)&str);
	return (final);
}
