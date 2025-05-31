/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_asterisk_mini.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 22:19:44 by carlopez          #+#    #+#             */
/*   Updated: 2025/06/01 00:03:43 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

static int	ft_asterisk2(char ***elements,
		char **s_input, int (*i)[3], int flag)
{
	while ((*elements)[(*i)[1]][(*i)[2]])
	{
		if ((*i)[0] == 0)
		{
			if (!ft_strncmp(&(*elements)[(*i)[1]][0],
				s_input[(*i)[0]], ft_strlen(s_input[(*i)[0]]) - 1))
			{
				(*i)[2] = (*i)[2] + ft_strlen(s_input[(*i)[0]]) - 1;
				return (1);
			}
			return (-1);
		}
		if (!ft_strncmp(&(*elements)[(*i)[1]][(*i)[2]],
			s_input[(*i)[0]], ft_strlen(s_input[(*i)[0]]) - 1))
		{
			(*i)[2] = (*i)[2] + ft_strlen(s_input[(*i)[0]]) - 1;
			return (1);
		}
		(*i)[2]++;
	}
	return (flag);
}

static int	ft_asterisk4(char ***elements,
		char **s_input, int (*i)[3], int *flag)
{
	*flag = ft_asterisk2(elements, s_input, i, *flag);
	if ((!(*elements)[(*i)[1]][(*i)[2]] && !*flag) || *flag == -1)
	{
		if (!ft_delete_item(elements, (*elements)[(*i)[1]]))
			return (0);
		*flag = 1;
		return (2);
	}
	else
		(*i)[0]++;
	return (1);
}

static int	ft_asterisk3(char ***elements,
		char **s_input, int (*i)[3], int *flag)
{
	while ((*elements)[(*i)[1]][(*i)[2]])
	{
		if (!ft_strncmp(&(*elements)[(*i)[1]][(*i)[2]],
				s_input[(*i)[0]], ft_strlen(s_input[(*i)[0]]) + 1))
		{
			*flag = 1;
			break ;
		}
		(*i)[2]++;
	}
	if (!(*elements)[(*i)[1]][(*i)[2]] && !*flag)
	{
		if (!ft_delete_item(elements, (*elements)[(*i)[1]]))
			return (0);
		*flag = 1;
		return (2);
	}
	else
		(*i)[0]++;
	return (1);
}

int	ft_main_asterisk(char **s_input, char ***elements, int (*i)[3], int *flag)
{
	int	res;

	while (s_input[(*i)[0]])
	{
		if (!ft_strncmp(s_input[(*i)[0]], "*", 2))
			(*i)[0]++;
		else if (ft_strchr(s_input[(*i)[0]], '*'))
		{
			res = ft_asterisk4(elements, s_input, i, flag);
			if (!res)
				return (0);
			else if (res == 2)
				break ;
		}
		else
		{
			res = ft_asterisk3(elements, s_input, i, flag);
			if (!res)
				return (0);
			else if (res == 2)
				break ;
		}
		*flag = 0;
	}
	return (1);
}

int	ft_check_asterisk(char *input, char ***elements)
{
	char	**s_input;
	int		i[3];
	int		flag;

	ft_init_int(&i);
	flag = 0;
	s_input = ft_split_asterisk(input, '*');
	if (!s_input || !*s_input)
		return (0);
	while ((*elements)[i[1]])
	{
		if (!ft_main_asterisk(s_input, elements, &i, &flag))
			return (0);
		i[0] = 0;
		i[2] = 0;
		if (flag != 1)
			i[1]++;
		flag = 0;
	}
	return (1);
}
