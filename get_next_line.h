/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melmbaz <melmbaz@student.42istanbul.com.tr +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 07:19:39 by melmbaz           #+#    #+#             */
/*   Updated: 2026/02/15 20:04:03 by melmbaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif
# include <stdlib.h>
# include <unistd.h>

typedef struct s_str
{
	char	*str;
	size_t	len;
}			t_str;

void		*join_blocks(t_str *strs, size_t len);
void		*ft_calloc(size_t nmemb, size_t size);
char		*get_next_line(int fd);
char		*ft_strchr(const char *s, int c);
char		*ft_strdup(const char *s);
size_t		ft_strlen(const char *str);

#endif