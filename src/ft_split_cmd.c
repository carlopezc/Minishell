/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 09:31:50 by lbellmas          #+#    #+#             */
/*   Updated: 2025/05/07 12:21:07 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../printf/libft/libft.h"
#include "../header/ft_minishell.h"

static int	ft_word_count(char const *w, char c)
{
	int	count;
	int	in_word;
	int	p;
	t_quote quote;

	p = 0;
	count = 0;
	in_word = 0;
	quote.type = 0;
	quote.flag = 0;
	while (w[p] != '\0')
	{
		//lo de pasar el puntero de la posicion es porque me da pereza cambiar los parametros que recibe pero no se usa luego los cambio
		ft_check_quote(&quote, w[p], &p);
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

void	ft_free_todo(int p, char **word)
{
	while (p >= 0)
	{
		free (word[p]);
		p--;
	}
	free (word);
	return ;
}

static char	**ft_cpy(char const *s, char **word, char c, int count)
{
	size_t	start;
	size_t	end;
	int		p;
	t_quote quote;

	p = 0;
	start = 0;
	quote.flag = 0;
	quote.type = 0;
	while (p < count)
	{
		while (s[start] == c)
			start++;
		end = start;
		while (((s[end] != c) && (s[end] != '\0')) || ((s[end] == c) && (quote.flag == 1)))
		{
			end++;
			ft_check_quote(&quote, s[end], 0);
		}
		word[p] = ft_substr(s, start, (end - start));
		if (!word[p])
		{
			ft_free_todo(p, word);
			return (NULL);
		}
		start = end;
		p++;
	}
	return (word);
}

char	**ft_split_cmd(char const *s, char c)
{
	char	**word;
	int		count;

	count = ft_word_count(s, c);
	word = (char **)malloc((count + 1) * sizeof(char *));
	if (!word)
		return (NULL);
	word = ft_cpy(s, word, c, count);
	word[count] = NULL;
	return (word);
}
/*
int	main()
{
	int	p = 0;
	char	**h = (ft_split_cmd("export hola=guapo hi perro $USER='que tal' ", ' '));
	
	while (h[p])
	{
		printf("%s", h[p]);
		printf("%c", '\n');
		p++;
	}
	printf("%s", h[p]);
	return (0);
}
*/
