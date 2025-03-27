/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:47:17 by lbellmas          #+#    #+#             */
/*   Updated: 2025/03/27 17:55:39 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

void	ft_clear_split(char **split)
{
	int 	p;

	if (!split)
		return ;
	p = 0;
	while (split[p])
	{
		free(split[p]);
		p++;
	}
	free(split);
}
