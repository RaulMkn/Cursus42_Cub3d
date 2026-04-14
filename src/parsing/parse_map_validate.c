/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_validate.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 00:00:00 by ruben             #+#    #+#             */
/*   Updated: 2026/04/09 10:48:07 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void	register_spawn(t_parse *p, int x, int y, char c)
{
	p->spawn_found++;
	p->spawn_x = x;
	p->spawn_y = y;
	p->spawn_dir = c;
}

static int	check_chars_and_spawn(t_parse *p)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	while (i < p->map_h)
	{
		j = 0;
		while (j < p->map_w)
		{
			c = p->map[i][j];
			if (!(c == '0' || c == '1' || c == 'N'
					|| c == 'S' || c == 'E' || c == 'W' || c == ' '))
				return (parse_error(p, "Invalid character in map"));
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
				register_spawn(p, j, i, c);
			j++;
		}
		i++;
	}
	if (p->spawn_found != 1)
		return (parse_error(p, "Map must have exactly one spawn"));
	return (0);
}

static int	is_open_cell(t_parse *p, int x, int y)
{
	if (x <= 0 || y <= 0 || x >= p->map_w - 1 || y >= p->map_h - 1)
		return (1);
	if (p->map[y - 1][x] == ' ' || p->map[y + 1][x] == ' ')
		return (1);
	if (p->map[y][x - 1] == ' ' || p->map[y][x + 1] == ' ')
		return (1);
	return (0);
}

static int	check_map_closed(t_parse *p)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	while (i < p->map_h)
	{
		j = 0;
		while (j < p->map_w)
		{
			c = p->map[i][j];
			if ((c == '0' || c == 'N' || c == 'S'
					|| c == 'E' || c == 'W') && is_open_cell(p, j, i))
				return (parse_error(p, "Map is not closed"));
			j++;
		}
		i++;
	}
	return (0);
}

int	validate_map(t_parse *p)
{
	if (check_chars_and_spawn(p) != 0)
		return (1);
	if (check_map_closed(p) != 0)
		return (1);
	return (0);
}
