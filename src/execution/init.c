/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 02:46:21 by rmakende          #+#    #+#             */
/*   Updated: 2026/04/14 21:12:57 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	load_one_texture(t_game *game, t_img *img, char *path)
{
	if (!path)
		return (1);
	img->img_ptr = mlx_xpm_file_to_image(game->mlx_ptr, path,
			&img->width, &img->height);
	if (!img->img_ptr)
		return (1);
	img->addr = mlx_get_data_addr(img->img_ptr, &img->bpp,
			&img->line_len, &img->endian);
	if (!img->addr)
		return (1);
	return (0);
}

int	init_mlx(t_game *game)
{
	if (!game)
		return (1);
	game->mlx_ptr = mlx_init();
	if (!game->mlx_ptr)
		return (1);
	if (game->win_width <= 0)
		game->win_width = WIN_WIDTH;
	if (game->win_height <= 0)
		game->win_height = WIN_HEIGHT;
	return (0);
}

int	init_window(t_game *game)
{
	game->win_ptr = mlx_new_window(game->mlx_ptr,
			game->win_width, game->win_height, "cub3D");
	if (!game->win_ptr)
		return (1);
	return (0);
}

int	init_screen(t_game *game)
{
	game->screen.width = game->win_width;
	game->screen.height = game->win_height;
	game->screen.img_ptr = mlx_new_image(game->mlx_ptr,
			game->screen.width, game->screen.height);
	if (!game->screen.img_ptr)
		return (1);
	game->screen.addr = mlx_get_data_addr(game->screen.img_ptr,
			&game->screen.bpp, &game->screen.line_len, &game->screen.endian);
	if (!game->screen.addr)
		return (1);
	return (0);
}

int	load_textures(t_game *game)
{
	if (load_one_texture(game, &game->tex[TEX_NO], game->no_texture) != 0)
		return (1);
	if (load_one_texture(game, &game->tex[TEX_SO], game->so_texture) != 0)
		return (1);
	if (load_one_texture(game, &game->tex[TEX_WE], game->we_texture) != 0)
		return (1);
	if (load_one_texture(game, &game->tex[TEX_EA], game->ea_texture) != 0)
		return (1);
	return (0);
}
