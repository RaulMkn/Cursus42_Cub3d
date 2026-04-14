/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_start.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 00:00:00 by ruortiz-          #+#    #+#             */
/*   Updated: 2026/04/13 00:00:00 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	init_failed(t_game *game)
{
	destroy_textures(game);
	destroy_screen(game);
	if (game->win_ptr)
	{
		mlx_destroy_window(game->mlx_ptr, game->win_ptr);
		game->win_ptr = NULL;
	}
	if (game->mlx_ptr)
	{
		mlx_destroy_display(game->mlx_ptr);
		free(game->mlx_ptr);
		game->mlx_ptr = NULL;
	}
	free_game_data(game);
	return (1);
}

int	game_loop(t_game *game)
{
	update_player(game);
	render_frame(game);
	mlx_put_image_to_window(game->mlx_ptr, game->win_ptr,
		game->screen.img_ptr, 0, 0);
	return (0);
}

int	game_start(t_game *game)
{
	if (init_mlx(game) != 0)
		return (init_failed(game));
	if (init_window(game) != 0)
		return (init_failed(game));
	if (init_screen(game) != 0)
		return (init_failed(game));
	if (load_textures(game) != 0)
		return (init_failed(game));
	mlx_hook(game->win_ptr, KeyPress, 1L << 0, on_key_press, game);
	mlx_hook(game->win_ptr, KeyRelease, 1L << 1, on_key_release, game);
	mlx_hook(game->win_ptr, DestroyNotify, 0, on_destroy, game);
	mlx_loop_hook(game->mlx_ptr, game_loop, game);
	mlx_loop(game->mlx_ptr);
	return (0);
}
