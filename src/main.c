/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 01:10:39 by rmakende          #+#    #+#             */
/*   Updated: 2026/04/14 21:13:16 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "../include/parser.h"

static void	init_game_fields(t_game *game)
{
	game->map = NULL;
	game->map_width = 0;
	game->map_height = 0;
	game->no_texture = NULL;
	game->so_texture = NULL;
	game->we_texture = NULL;
	game->ea_texture = NULL;
	game->floor_color = 0;
	game->ceiling_color = 0;
	game->player_x = 0;
	game->player_y = 0;
	game->dir_x = 0;
	game->dir_y = 0;
	game->plane_x = 0;
	game->plane_y = 0;
}

static void	init_game_win(t_game *game)
{
	int	i;

	game->mlx_ptr = NULL;
	game->win_ptr = NULL;
	game->win_width = WIN_WIDTH;
	game->win_height = WIN_HEIGHT;
	game->screen.img_ptr = NULL;
	i = 0;
	while (i < 4)
	{
		game->tex[i].img_ptr = NULL;
		i++;
	}
	game->keys.w = 0;
	game->keys.s = 0;
	game->keys.a = 0;
	game->keys.d = 0;
	game->keys.left = 0;
	game->keys.right = 0;
}

static void	init_game_zero(t_game *game)
{
	init_game_fields(game);
	init_game_win(game);
}

static char	*get_map_path(int argc, char **argv, int *from_menu)
{
	char	*path;

	*from_menu = 0;
	if (argc == 1)
	{
		path = show_map_menu();
		*from_menu = 1;
		return (path);
	}
	if (argc == 2)
		return (argv[1]);
	ft_printf("Usage: %s [map.cub]\n", argv[0]);
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_game	game;
	char	*map_path;
	int		from_menu;

	map_path = get_map_path(argc, argv, &from_menu);
	if (!map_path)
		return (1);
	init_game_zero(&game);
	if (parse_cub_file(map_path, &game) != 0)
	{
		ft_printf("Error\nFailed to parse map file: %s\n", map_path);
		if (from_menu)
			free(map_path);
		return (1);
	}
	if (from_menu)
		free(map_path);
	if (game_start(&game) != 0)
	{
		write(2, "Error\nFailed to start game.\n", 29);
		free_game_data(&game);
		return (1);
	}
	free_game_data(&game);
	return (0);
}
