/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbellmas <lbellmas@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:37:43 by lbellmas          #+#    #+#             */
/*   Updated: 2025/02/10 10:45:38 by lbellmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <stdlib.h>
# include <stdio.h>
# include <limits.h>
# include <unistd.h>
# include <fcntl.h>
# include "../printf/header/ft_printf.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

# ifndef OPEN_MAX
#  define OPEN_MAX 1024
# endif

char	*get_next_line(int fd);
size_t	ft_get_strlen(const char *str);
void	ft_get_bzero(void *s, size_t n);
char	*ft_get_strchr(const char *s, int c);
char	*ft_get_strjoin(char *s1, char const *s2);
char	*ft_get_strdup(char *s);
size_t	ft_get_strlcpy(char *dst, const char *src, size_t size);
char	*ft_read_join(char **rest, char *str_final, int fd);
char	*ft_rest(char **rest, int fd);
char	*free_and_nullify(char **s1, char **s2);

#endif
