/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_math.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 00:00:00 by ruortiz-          #+#    #+#             */
/*   Updated: 2026/04/13 00:00:00 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	select_texture(t_ray *ray)
{
	if (ray->side == 0 && ray->ray_dir_x > 0)
		ray->tex_index = TEX_WE;
	else if (ray->side == 0 && ray->ray_dir_x < 0)
		ray->tex_index = TEX_EA;
	else if (ray->side == 1 && ray->ray_dir_y > 0)
		ray->tex_index = TEX_NO;
	else
		ray->tex_index = TEX_SO;
}

static void	set_texture_coord(t_game *game, t_ray *ray)
{
	int	tex_w;

	if (ray->side == 0)
		ray->wall_x = game->player_y + ray->perp_dist * ray->ray_dir_y;
	else
		ray->wall_x = game->player_x + ray->perp_dist * ray->ray_dir_x;
	ray->wall_x -= floor(ray->wall_x);
	tex_w = game->tex[ray->tex_index].width;
	ray->tex_x = (int)(ray->wall_x * (double)tex_w);
	if (ray->side == 0 && ray->ray_dir_x > 0)
		ray->tex_x = tex_w - ray->tex_x - 1;
	if (ray->side == 1 && ray->ray_dir_y < 0)
		ray->tex_x = tex_w - ray->tex_x - 1;
}

void	ray_set_start(t_game *game, t_ray *ray, int x)
{
	ray->camera_x = 2.0 * x / (double)game->win_width - 1.0;
	ray->ray_dir_x = game->dir_x + game->plane_x * ray->camera_x;
	ray->ray_dir_y = game->dir_y + game->plane_y * ray->camera_x;
	ray->map_x = (int)game->player_x;
	ray->map_y = (int)game->player_y;
	if (ray->ray_dir_x == 0)
		ray->delta_dist_x = 1e30;
	else
		ray->delta_dist_x = fabs(1.0 / ray->ray_dir_x);
	if (ray->ray_dir_y == 0)
		ray->delta_dist_y = 1e30;
	else
		ray->delta_dist_y = fabs(1.0 / ray->ray_dir_y);
	ray->hit = 0;
}

void	ray_set_projection(t_game *game, t_ray *ray)
{
	double	offset;

	if (ray->side == 0)
	{
		offset = ray->map_x - game->player_x + (1 - ray->step_x) / 2.0;
		ray->perp_dist = offset / ray->ray_dir_x;
	}
	else
	{
		offset = ray->map_y - game->player_y + (1 - ray->step_y) / 2.0;
		ray->perp_dist = offset / ray->ray_dir_y;
	}
	ray->line_height = (int)(game->win_height / ray->perp_dist);
	ray->draw_start = -ray->line_height / 2 + game->win_height / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = ray->line_height / 2 + game->win_height / 2;
	if (ray->draw_end >= game->win_height)
		ray->draw_end = game->win_height - 1;
}

void	ray_set_sampling(t_game *game, t_ray *ray)
{
	int		offset;

	select_texture(ray);
	set_texture_coord(game, ray);
	ray->tex_step = 1.0 * game->tex[ray->tex_index].height / ray->line_height;
	offset = ray->draw_start - game->win_height / 2 + ray->line_height / 2;
	ray->tex_pos = offset * ray->tex_step;
}
