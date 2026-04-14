/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 00:00:00 by ruortiz-          #+#    #+#             */
/*   Updated: 2026/04/13 00:00:00 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	row_len(char *row)
{
	int	len;

	len = 0;
	while (row && row[len])
		len++;
	return (len);
}

int	is_wall(t_game *game, double x, double y)
{
	int		mx;
	int		my;
	char	cell;

	mx = (int)x;
	my = (int)y;
	if (mx < 0 || my < 0 || mx >= game->map_width || my >= game->map_height)
		return (1);
	if (!game->map[my])
		return (1);
	if (mx >= row_len(game->map[my]))
		return (1);
	cell = game->map[my][mx];
	if (cell == '1' || cell == ' ' || cell == '\0')
		return (1);
	return (0);
}
