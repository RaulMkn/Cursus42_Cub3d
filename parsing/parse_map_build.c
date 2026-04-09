/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_build.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 00:00:00 by ruben             #+#    #+#             */
/*   Updated: 2026/04/08 14:34:59 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	find_map_width(t_parse *p, int start_idx)
{
	int	max;
	int	len;
	int	i;

	max = 0;
	i = start_idx;
	while (i < p->line_count)
	{
		len = (int)ft_strlen(p->lines[i]);
		if (len > max)
			max = len;
		i++;
	}
	return (max);
}

static int	find_map_height(t_parse *p, int start_idx)
{
	int	count;
	int	i;

	count = 0;
	i = start_idx;
	while (i < p->line_count)
	{
		if (is_empty_line(p->lines[i]))
		{
			i++;
			while (i < p->line_count)
			{
				if (!is_empty_line(p->lines[i]))
					return (-1);
				i++;
			}
			break ;
		}
		else
		{
			count++;
			i++;
		}
	}
	return (count);
}

static char	*build_map_row(const char *src, int width)
{
	char	*row;
	int		src_len;
	int		i;

	row = malloc(sizeof(char) * (width + 1));
	if (!row)
		return (NULL);
	src_len = (int)ft_strlen(src);
	i = 0;
	while (i < src_len && i < width)
	{
		row[i] = src[i];
		i++;
	}
	while (i < width)
	{
		row[i] = ' ';
		i++;
	}
	row[width] = '\0';
	return (row);
}

int	build_map(t_parse *p, int start_idx)
{
	int	i;

	p->map_w = find_map_width(p, start_idx);
	p->map_h = find_map_height(p, start_idx);
	if (p->map_h <= 0 || p->map_w <= 0)
		return (parse_error(p, "Empty map or empty line inside map block"));
	p->map = malloc(sizeof(char *) * (p->map_h + 1));
	if (!p->map)
		return (parse_error(p, "Memory allocation failed for map"));
	i = 0;
	while (i < p->map_h)
	{
		p->map[i] = build_map_row(p->lines[start_idx + i], p->map_w);
		if (!p->map[i])
		{
			p->map[i + 1] = NULL;
			return (parse_error(p, "Memory allocation failed for map row"));
		}
		i++;
	}
	p->map[p->map_h] = NULL;
	return (0);
}
