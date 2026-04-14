/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 00:00:00 by ruben             #+#    #+#             */
/*   Updated: 2026/04/08 14:21:50 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	get_header_id(const char *line)
{
	if (ft_strncmp(line, "NO ", 3) == 0 || ft_strncmp(line, "NO\t", 3) == 0)
		return (0);
	else if (ft_strncmp(line, "SO ", 3) == 0
		|| ft_strncmp(line, "SO\t", 3) == 0)
		return (1);
	else if (ft_strncmp(line, "WE ", 3) == 0
		|| ft_strncmp(line, "WE\t", 3) == 0)
		return (2);
	else if (ft_strncmp(line, "EA ", 3) == 0
		|| ft_strncmp(line, "EA\t", 3) == 0)
		return (3);
	else if (ft_strncmp(line, "F ", 2) == 0 || ft_strncmp(line, "F\t", 2) == 0)
		return (4);
	else if (ft_strncmp(line, "C ", 2) == 0 || ft_strncmp(line, "C\t", 2) == 0)
		return (5);
	else
		return (-1);
}

static int	parse_one_header(t_parse *p, const char *line)
{
	int	id;

	id = get_header_id(line);
	if (id >= 0 && id <= 3)
		return (parse_texture_line(p, line, id));
	else if (id == 4 || id == 5)
		return (parse_color_line(p, line, id));
	else
		return (parse_error(p, "Unknown header identifier"));
}

static int	all_headers_found(t_parse *p)
{
	return (p->no != NULL && p->so != NULL && p->we != NULL && p->ea != NULL
		&& p->floor_color != -1 && p->ceiling_color != -1);
}

int	parse_header(t_parse *p)
{
	int		i;
	char	*line;

	i = 0;
	while (i < p->line_count)
	{
		line = skip_spaces(p->lines[i]);
		if (is_empty_line(line))
			i++;
		else if (get_header_id(line) != -1)
		{
			if (parse_one_header(p, line) != 0)
				return (-1);
			i++;
		}
		else
			break ;
	}
	if (!all_headers_found(p))
	{
		parse_error(p, "Missing header identifiers (NO/SO/WE/EA/F/C)");
		return (-1);
	}
	return (i);
}
