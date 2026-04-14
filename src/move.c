/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 00:00:00 by ruortiz-          #+#    #+#             */
/*   Updated: 2026/04/13 00:00:00 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	move_axis(t_game *game, double nx, double ny)
{
	if (!is_wall(game, nx, game->player_y))
		game->player_x = nx;
	if (!is_wall(game, game->player_x, ny))
		game->player_y = ny;
}

void	move_forward_backward(t_game *game, double speed)
{
	double	nx;
	double	ny;

	nx = game->player_x + game->dir_x * speed;
	ny = game->player_y + game->dir_y * speed;
	move_axis(game, nx, ny);
}

void	move_strafe(t_game *game, double speed)
{
	double	nx;
	double	ny;

	nx = game->player_x + game->dir_y * speed;
	ny = game->player_y - game->dir_x * speed;
	move_axis(game, nx, ny);
}

void	rotate_player(t_game *game, double angle)
{
	double	old_dir_x;
	double	old_plane_x;
	double	cos_a;
	double	sin_a;

	cos_a = cos(angle);
	sin_a = sin(angle);
	old_dir_x = game->dir_x;
	game->dir_x = game->dir_x * cos_a - game->dir_y * sin_a;
	game->dir_y = old_dir_x * sin_a + game->dir_y * cos_a;
	old_plane_x = game->plane_x;
	game->plane_x = game->plane_x * cos_a - game->plane_y * sin_a;
	game->plane_y = old_plane_x * sin_a + game->plane_y * cos_a;
}
