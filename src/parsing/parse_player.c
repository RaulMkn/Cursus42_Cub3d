/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_player.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 00:00:00 by ruben             #+#    #+#             */
/*   Updated: 2026/04/09 10:47:48 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void	set_player_position(t_parse *p, t_game *game)
{
	game->player_x = p->spawn_x + 0.5;
	game->player_y = p->spawn_y + 0.5;
}

static void	set_dir_ns(char dir, t_game *game)
{
	game->dir_x = 0;
	game->plane_y = 0;
	if (dir == 'N')
	{
		game->dir_y = -1;
		game->plane_x = 0.66;
	}
	else
	{
		game->dir_y = 1;
		game->plane_x = -0.66;
	}
}

static void	set_dir_ew(char dir, t_game *game)
{
	game->dir_y = 0;
	game->plane_x = 0;
	if (dir == 'E')
	{
		game->dir_x = 1;
		game->plane_y = 0.66;
	}
	else
	{
		game->dir_x = -1;
		game->plane_y = -0.66;
	}
}

static void	set_player_direction(char dir, t_game *game)
{
	if (dir == 'N' || dir == 'S')
		set_dir_ns(dir, game);
	else
		set_dir_ew(dir, game);
}

void	init_player(t_parse *p, t_game *game)
{
	set_player_position(p, game);
	set_player_direction(p->spawn_dir, game);
	p->map[p->spawn_y][p->spawn_x] = '0';
}
