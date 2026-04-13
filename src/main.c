#include "cub3d.h"
#include "../parsing/parser.h"
#include <stdio.h>

static void	init_game_zero(t_game *game)
{
	int	i;

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

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
	{
		ft_printf("Usage: %s <map.cub>\n", argv[0]);
		return (1);
	}
	init_game_zero(&game);
	if (parse_cub_file(argv[1], &game) != 0)
	{
		ft_printf("Error\nFailed to parse map file: %s\n", argv[1]);
		return (1);
	}
	if (game_start(&game) != 0)
	{
		write(2, "Error\nFailed to start game (check textures/display)\n", 52);
		free_game_data(&game);
		return (1);
	}
	free_game_data(&game);
	return (0);
}
