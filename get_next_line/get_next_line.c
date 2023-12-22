/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykarakur <ykarakur@student.42kocaeli.com.  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 14:39:33 by ykarakur          #+#    #+#             */
/*   Updated: 2023/12/13 14:40:20 by ykarakur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*backup_stash(char *stash, char *buf)
{
	char	*res;

	res = 0;
	if (!stash && buf)
	{
		res = ft_strdup(buf);
		if (!res)
			return (NULL);
		return (res);
	}
	res = ft_strjoin(stash, buf);
	ft_free_stash(&stash, 0);
	return (res);
}

static int	check_newline(char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = -1;
	while (s[++i] != '\0')
		if (s[i] == '\n')
			return (1);
	return (0);
}

static char	*extract_line(char *stash)
{
	char	*line;
	size_t	i;
	size_t	j;

	i = 0;
	if (!stash)
		return (0);
	while (stash[i] != '\n')
		i++;
	line = malloc(sizeof(char) * (i + 2));
	if (!line)
		return (0);
	j = 0;
	while (j < i + 1)
	{
		line[j] = stash[j];
		j++;
	}
	line[j] = '\0';
	return (line);
}

static char	*remain_stash(char *stash)
{
	size_t	i;
	char	*new_stash;

	i = 0;
	if (!stash)
		return (NULL);
	while (stash[i] != '\n')
		i++;
	if (stash[i + 1] == '\0')
		return (ft_free_stash(&stash, 0));
	new_stash = ft_substr(stash, i + 1, ft_strlen(stash));
	if (!new_stash)
	{
		ft_free_stash(&stash, 0);
		return (NULL);
	}
	ft_free_stash(&stash, 0);
	return (new_stash);
}

char	*get_next_line(int fd)
{
	char		buf[BUFFER_SIZE + 1];
	ssize_t		read_size;
	static char	*stash = NULL;
	char		*line;

	line = 0;
	read_size = BUFFER_SIZE;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (ft_free_stash(&stash, 0));
	while (read_size > 0)
	{
		read_size = read(fd, buf, BUFFER_SIZE);
		if ((read_size <= 0 && !stash) || read_size == -1)
			return (ft_free_stash(&stash, 0));
		buf[read_size] = '\0';
		stash = backup_stash(stash, buf);
		if (check_newline(stash))
		{
			line = extract_line(stash);
			if (!line)
				return (ft_free_stash(&stash, 0));
			return (stash = remain_stash(stash), line);
		}
	}
	return (ft_free_stash(&stash, 1));
}
