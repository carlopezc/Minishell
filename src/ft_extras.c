/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_extras.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 20:49:32 by lbellmas          #+#    #+#             */
/*   Updated: 2025/07/24 21:41:19 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_minishell.h"

int	ft_get_size(char *input, char c)
{
	int		i;
	int		size;

	i = 0;
	size = 0;
	while (input[i])
	{
		if (input[i] == c)
			size++;
		i++;
	}
	if (input[i - 1] != c)
		size++;
	return (size);
}

int	ft_find_asterisk_in_word(const char *str, int index)
{
	int	start;
	int	end;
	int	i;

	start = index;
	end = index;
	while (start > 0 && str[start - 1] != ' ')
		start--;
	while (str[end] && str[end] != ' ')
		end++;
	i = start;
	while (i < end)
	{
		if (str[i] == '*')
			return (1);
		i++;
	}
	return (0);
}

char	*ft_expand_wildcard(char *input, t_token_type type)
{
	char	**elements;
	char	*elements_input;

	elements = ft_get_elements();
	if (!elements)
		return (ft_free_array(elements), NULL);
	if (!ft_strncmp(input, "*", 2))
	{
		elements_input = ft_create_array(elements);
		return (ft_free_array(elements), elements_input);
	}
	if (!ft_check_asterisk(input, &elements))
		return (NULL);
	if ((type == REDIR_IN || type == REDIR_OUT) && (ft_arraylen(elements) > 1))
		return (ft_free_array(elements),
			ft_printf("Error in files to redirect\n"), NULL);
	if (!ft_arraylen(elements))
		return (ft_free_array(elements), ft_strdup(input));
	elements_input = ft_create_array(elements);
	return (ft_free_array(elements), elements_input);
}

int	ft_cmp_except(char *str, char *str2, int n)
{
	if (!ft_strncmp(str, str2, n - 1)
		&& (!str[n - 1] || (str[n - 1] == ' ' || str[n - 1] == ')')))
		return (0);
	return (1);
}

t_env	*ft_search_pwd(t_minishell *shell)
{
	t_env	*pwd;

	pwd = shell->env;
	while (pwd && pwd->next && ft_strncmp((pwd->next)->name,
			"PWD", ft_max_strlen("PWD", (pwd->next)->name)))
		pwd = pwd->next;
	return (pwd);
}
