/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melmbaz <melmbaz@student.42istanbul.com.tr +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 07:19:46 by melmbaz           #+#    #+#             */
/*   Updated: 2026/02/16 22:44:36 by melmbaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strdup(const char *s)
{
	size_t	s_len;
	char	*arr;
	char	*start;

	s_len = ft_strlen(s);
	arr = (char *)malloc(sizeof(char) * (s_len + 1));
	if (!arr)
		return (NULL);
	start = arr;
	while (*s)
		*arr++ = *s++;
	*arr = '\0';
	return (start);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*arr;
	size_t	i;

	i = 0;
	if (size != 0 && nmemb > (size_t)-1 / size)
		return (NULL);
	arr = malloc(nmemb * size);
	if (!arr)
		return (NULL);
	if (nmemb == 0 || size == 0)
		return (arr);
	while (i < nmemb * size)
		((unsigned char *)arr)[i++] = 0;
	return (arr);
}

char	*ft_strchr(const char *s, int c)
{
	size_t	i;

	i = 0;
	if ((unsigned char)c == '\0')
	{
		while (s[i])
			i++;
		return ((char *)(s + i));
	}
	while (*s)
	{
		if ((unsigned char)*s++ == (unsigned char)c)
			return ((char *)(s - 1));
	}
	return (NULL);
}

void	*join_blocks(t_str *strs, size_t block_len)
{
	char	*memory;
	char	*temp_memory;
	size_t	total_len;
	size_t	i;
	size_t	j;

	total_len = 0;
	i = 0;
	while (i < block_len)
		total_len += strs[i++].len;
	memory = malloc(total_len + 1);
	if (!memory)
		return (NULL);
	temp_memory = memory;
	temp_memory[total_len] = '\0';
	i = 0;
	while (i < block_len)
	{
		j = 0;
		while (j < strs[i].len)
			*memory++ = strs[i].str[j++];
		i++;
	}
	return (temp_memory);
}
