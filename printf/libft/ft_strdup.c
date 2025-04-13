/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 11:34:56 by lbellmas          #+#    #+#             */
/*   Updated: 2025/04/13 19:32:43 by carlotalcd       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*p;
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	p = (char *)malloc((len + 1) * sizeof(char));
	if (!p)
		return (NULL);
	p = ft_memcpy(p, s, (len + 1));
	return (p);
}

/*int	main()
{
	printf("%s", ft_strdup("hola"));
	return (0);
}*/
