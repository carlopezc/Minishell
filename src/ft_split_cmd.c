/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 09:31:50 by lbellmas          #+#    #+#             */
/*   Updated: 2025/05/14 15:01:56 by carlopez         ###   ########.fr       */
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
		if (!w[p - 1] || w[p - 1] != '\\')
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

int	ft_count_letters(char const *s, char c, int i)
{
	int	letters;
	t_quote	quote;

	letters = 0;
	quote.flag = 0;
	quote.type = 0;
	while (s[i] && s[i] == c)
		(i)++;
	while (s[i] && (s[i] != c || (s[i] == c && quote.flag == 1)))
	{
		if (!s[i - 1] || s[i - 1] != '\\')
			ft_check_quote(&quote, s[i], &i);
		(i)++;
		letters++;
	}
	return (letters);
}

static char	**ft_cpy(char const *s, char **word, char c, int count)
{
	int		p;
	t_quote quote;
	int	chars;
	int	i;
	int	j;

	p = 0;
	i = 0;
	j = 0;
	chars = 0;
	quote.flag = 0;
	quote.type = 0;
	//Hay que arreglar esto, al hacerlo con substr se cagaaa por ej con "exp"
	while (p < count)
	{
		chars = ft_count_letters(s, c, i);
		word[p] = (char *)malloc((chars + 1) * sizeof(char));
		if (!word[p])
			return (ft_free_todo(p, word), NULL);
		while (s[i] == c)
			i++;
		if (!s[i - 1] || s[i - 1] != '\\')
			ft_check_quote(&quote, s[i], &i);
		while (s[i] && (s[i] != c || (s[i] == c && quote.flag == 1)))
		{
			word[p][j++] = s[i++];
			if (!s[i - 1] || s[i - 1] != '\\')
				ft_check_quote(&quote, s[i], &i);
		}
		word[p][j] = '\0';
		j = 0;
		p++;
	}
	word[p] = NULL;
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
