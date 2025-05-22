/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcard.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:57:16 by carlopez          #+#    #+#             */
/*   Updated: 2025/05/22 18:27:32 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

char	**ft_get_elements(void)
{
	char	**elements;
	DIR	*dir;

	files = NULL;
	dir = opendir(".");
	if (!dir)
		return (ft_printf("Error opening dir\n"), NULL);
}

char	*ft_expand_wildcard(char *input)
{
	char	**elements;
	
	elements = ft_get_elements();
	if (!elements)
		return (NULL);

}

int	ft_check_wildcard(char **input)
{
	int	i;
	char	*final_input;
	char	**s_input;
	char	*asterisk;

	i = 0;
	final_input = NULL;
	s_input = ft_split(*input, ' ');
	if (!s_input || !*s_input)
		return (0);
	while (s_input[i])
	{
		asterisk = ft_strchr(s_input[i], '*');
		if (asterisk && (!*(asterisk - 1) || *(asterisk - 1) != '\\'))
			s_input[i] = ft_expand_wildcard(s_input[i]); 
		if (!s_input[i])
			return (0);
		i++;
	}
	final_input = ft_create_array(s_input);
	if (!final_input)
		return (0);
	return (1);
}
