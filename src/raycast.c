/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 00:00:00 by ruortiz-          #+#    #+#             */
/*   Updated: 2026/04/13 00:00:00 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	perform_dda(t_game *game, t_ray *ray)
{
	while (ray->hit == 0)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (is_wall(game, ray->map_x, ray->map_y))
			ray->hit = 1;
	}
}

static void	set_step_and_side_dist(t_game *game, t_ray *ray)
{
	double	dx;
	double	dy;

	if (ray->ray_dir_x < 0)
	{
		ray->step_x = -1;
		dx = game->player_x - ray->map_x;
	}
	else
	{
		ray->step_x = 1;
		dx = ray->map_x + 1.0 - game->player_x;
	}
	ray->side_dist_x = dx * ray->delta_dist_x;
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		dy = game->player_y - ray->map_y;
	}
	else
	{
		ray->step_y = 1;
		dy = ray->map_y + 1.0 - game->player_y;
	}
	ray->side_dist_y = dy * ray->delta_dist_y;
}

void	cast_one_ray(t_game *game, t_ray *ray, int x)
{
	ray_set_start(game, ray, x);
	set_step_and_side_dist(game, ray);
	perform_dda(game, ray);
	ray_set_projection(game, ray);
	ray_set_sampling(game, ray);
}
