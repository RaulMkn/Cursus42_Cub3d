/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 00:00:00 by ruben             #+#    #+#             */
/*   Updated: 2026/04/09 10:48:36 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	check_extension(const char *path)
{
	char	*dot;

	dot = ft_strrchr(path, '.');
	if (!dot || dot == path || *(dot - 1) == '/')
		return (1);
	if (ft_strncmp(dot, ".cub", 5) != 0)
		return (1);
	return (0);
}

static void	strip_newline(char *line)
{
	char	*newline_pos;

	newline_pos = ft_strchr(line, '\n');
	if (newline_pos)
		*newline_pos = '\0';
}

static int	count_file_lines(const char *path)
{
	int		fd;
	int		count;
	char	*line;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (-1);
	count = 0;
	line = get_next_line(fd);
	while (line)
	{
		count++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (count);
}

static void	read_into_lines(int fd, t_parse *p, int total)
{
	char	*line;

	p->line_count = 0;
	line = get_next_line(fd);
	while (line && p->line_count < total)
	{
		strip_newline(line);
		p->lines[p->line_count] = line;
		p->line_count++;
		line = get_next_line(fd);
	}
	if (line)
		free(line);
}

int	load_lines(const char *path, t_parse *p)
{
	int	total;
	int	fd;

	if (check_extension(path) != 0)
		return (parse_error(p, "Invalid file extension (expected .cub)"));
	total = count_file_lines(path);
	if (total <= 0)
		return (parse_error(p, "Empty or unreadable file"));
	p->lines = malloc(sizeof(char *) * (total + 1));
	if (!p->lines)
		return (parse_error(p, "Memory allocation failed"));
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (parse_error(p, "Cannot open file"));
	read_into_lines(fd, p, total);
	close(fd);
	return (0);
}
