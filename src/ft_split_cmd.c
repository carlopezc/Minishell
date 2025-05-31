/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 09:31:50 by lbellmas          #+#    #+#             */
/*   Updated: 2025/05/30 19:14:19 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../printf/libft/libft.h"
#include "../header/ft_minishell.h"

static int	ft_word_count(char const *w, char c)
{
	int		count;
	int		in_word;
	int		p;
	t_quote	quote;

	p = 0;
	count = 0;
	in_word = 0;
	quote.type = 0;
	quote.flag = 0;
	while (w && w[p])
	{
		if (p > 0 && (!w[p - 1] || w[p - 1] != '\\'))
			ft_check_quote(&quote, w[p]);
		if (w[p] != c && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (w[p] == c && quote.flag == 0)
			in_word = 0;
		p++;
	}
	return (count);
}

void	ft_free_all(int p, char **word)
{
	while (p >= 0)
	{
		free(word[p]);
		word[p] = NULL;
		p--;
	}
	free (word);
	return ;
}

int	ft_cpy(char const *s, char **word, char c, int count)
{
	int		p;
	t_quote	quote;
	int		i;

	p = -1;
	i = 0;
	ft_init_quote(&quote);
	while (++p < count)
	{
		word[p] = NULL;
		while (s[i] == c)
			i++;
		if (i > 0 && (!s[i - 1] || s[i - 1] != '\\'))
			ft_check_quote(&quote, s[i]);
		while (s[i] && (s[i] != c || (s[i] == c && quote.flag == 1)))
		{
			word[p] = ft_strjoin_char(word[p], s[i++]);
			if (!word[p])
				return (0);
			if (i > 0 && (!s[i - 1] || s[i - 1] != '\\'))
				ft_check_quote(&quote, s[i]);
		}
	}
	word[p] = NULL;
	return (1);
}

char	**ft_split_cmd(char const *s, char c)
{
	char	**word;
	int		count;

	count = ft_word_count(s, c);
	word = (char **)malloc((count + 1) * sizeof(char *));
	if (!word)
		return (NULL);
	if (!ft_cpy(s, word, c, count))
		return (NULL);
	return (word);
}
