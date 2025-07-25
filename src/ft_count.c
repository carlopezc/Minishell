/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlotalcd <carlotalcd@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 18:28:50 by carlopez          #+#    #+#             */
/*   Updated: 2025/07/25 14:45:35 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_count_files(DIR *dir)
{
	struct dirent	*entry;
	int				files;

	files = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.')
			files++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (files);
}

int	ft_count_brackets(char *str)
{
	int		i;
	int		o_bracket;
	int		c_bracket;
	t_quote	q;

	o_bracket = 0;
	c_bracket = 0;
	ft_init_quote(&q);
	i = 0;
	while (str[i])
	{
		if (str[i] && (str[i] == '\''
				|| str[i] == '\"') && (!i || str[i - 1] != '\\'))
			ft_check_quote(&q, str[i]);
		if (!q.flag && str[i] == '('
			&& (!i || (!str[i - 1] || str[i - 1] != '\\')))
			o_bracket++;
		else if (!q.flag && str[i] == ')'
			&& (!i || (!str[i - 1] || str[i - 1] != '\\')))
			c_bracket++;
		i++;
	}
	if (o_bracket == c_bracket)
		return (1);
	return (0);
}

int	ft_check_qorder(char *str)
{
	int	i;
	int	sing;
	int	doub;

	i = 0;
	sing = 0;
	doub = 0;
	if (!str)
		return (1);
	while (str[i])
	{
		if (str[i] == '\'' && (!i || str[i - 1] != '\\') && !doub)
			sing = !sing;
		else if (str[i] == '\"' && (!i || str[i - 1] != '\\') && !sing)
			doub = !doub;
		i++;
	}
	if (sing || doub)
		return (ft_printf("Error with open quotes\n"), 0);
	return (1);
}

int	ft_count_quotes(char *str)
{
	int	i;
	int	double_quotes;
	int	simple_quotes;

	i = 0;
	simple_quotes = 0;
	double_quotes = 0;
	while (str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
		{
			if (!i || (!str[i - 1] || (str[i - 1] && str[i - 1] != '\\')))
			{
				if (str[i] == '\"')
					double_quotes++;
				else
					simple_quotes++;
			}
		}
		i++;
	}
	if (!(double_quotes % 2) && !(simple_quotes % 2))
		return (ft_check_qorder(str));
	return (0);
}
