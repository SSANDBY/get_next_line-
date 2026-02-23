/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melmbaz <melmbaz@student.42istanbul.com.tr +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 07:19:43 by melmbaz           #+#    #+#             */
/*   Updated: 2026/02/18 11:16:47 by melmbaz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_gnl_end(char **saved, char **buff)
{
	char	*line;

	line = NULL;
	if (*saved && ft_strlen(*saved) > 0)
		line = ft_strdup(*saved);
	free(*saved);
	*saved = NULL;
	free(*buff);
	*buff = NULL;
	return (line);
}

static int	case_zero(int fd, char **saved, char *buff, int *cases)
{
	ssize_t	read_len;
	char	*old_saved;

	read_len = read(fd, buff, BUFFER_SIZE);
	if (read_len < 0)
		return (free(*saved), *saved = NULL, -1);
	if (read_len == 0)
		return (-1);
	buff[read_len] = '\0';
	old_saved = *saved;
	*saved = join_blocks((t_str[]){(t_str){.str = old_saved,
			.len = ft_strlen(old_saved)}, (t_str){.str = buff,
			.len = (size_t)read_len}}, 2);
	free(old_saved);
	if (!(*saved))
		return (-1);
	if (ft_strchr(*saved, '\n'))
	{
		*cases = 1;
		return (1);
	}
	return (0);
}

static char	*case_one(char **saved, int *cases)
{
	char	*new_line;
	char	*line;
	char	*temp;

	new_line = ft_strchr(*saved, '\n');
	if (!new_line)
	{
		*cases = 0;
		return (NULL);
	}
	line = join_blocks((t_str[]){(t_str){.str = *saved, .len = (size_t)(new_line
				- *saved + 1)}}, 1);
	if (!line)
		return (free(*saved), *saved = NULL, NULL);
	temp = ft_strdup(new_line + 1);
	if (!temp)
		return ((char *)(ft_gnl_end(saved, &line)));
	free(*saved);
	*saved = temp;
	*cases = 0;
	return (line);
}

static char	*reader(int fd, char **saved, char *buff, int *cases)
{
	if (ft_strchr(*saved, '\n'))
		return (case_one(saved, cases));
	while (*cases == 0)
		if (case_zero(fd, saved, buff, cases) == -1)
			return (NULL);
	return (case_one(saved, cases));
}

char	*get_next_line(int fd)
{
	static char	*buff;
	static char	*saved;
	static int	cases;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (ft_gnl_end(&saved, &buff));
	if (!buff)
		buff = ft_calloc(BUFFER_SIZE + 1, 1);
	if (!buff)
		return (NULL);
	if (!saved)
		saved = ft_strdup("");
	if (!saved)
	{
		free(buff);
		buff = NULL;
		return (NULL);
	}
	line = reader(fd, &saved, buff, &cases);
	if (line)
		return (line);
	return (ft_gnl_end(&saved, &buff));
}
