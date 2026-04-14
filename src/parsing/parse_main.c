/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 00:00:00 by ruben             #+#    #+#             */
/*   Updated: 2026/04/08 23:18:02 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void	init_parse(t_parse *p)
{
	p->lines = NULL;
	p->line_count = 0;
	p->no = NULL;
	p->so = NULL;
	p->we = NULL;
	p->ea = NULL;
	p->floor_color = -1;
	p->ceiling_color = -1;
	p->map = NULL;
	p->map_w = 0;
	p->map_h = 0;
	p->spawn_found = 0;
	p->spawn_x = 0;
	p->spawn_y = 0;
	p->spawn_dir = 0;
}

static int	transfer_to_game(t_parse *p, t_game *game)
{
	game->map = p->map;
	p->map = NULL;
	game->map_width = p->map_w;
	game->map_height = p->map_h;
	game->no_texture = p->no;
	p->no = NULL;
	game->so_texture = p->so;
	p->so = NULL;
	game->we_texture = p->we;
	p->we = NULL;
	game->ea_texture = p->ea;
	p->ea = NULL;
	game->floor_color = p->floor_color;
	game->ceiling_color = p->ceiling_color;
	game->mlx_ptr = NULL;
	game->win_ptr = NULL;
	game->win_width = WIN_WIDTH;
	game->win_height = WIN_HEIGHT;
	return (0);
}

int	parse_cub_file(const char *path, t_game *game)
{
	t_parse	p;
	int		map_start;

	init_parse(&p);
	if (load_lines(path, &p) != 0)
		return (1);
	map_start = parse_header(&p);
	if (map_start < 0)
		return (1);
	if (build_map(&p, map_start) != 0)
		return (1);
	if (validate_map(&p) != 0)
		return (1);
	init_player(&p, game);
	transfer_to_game(&p, game);
	free_parse(&p);
	return (0);
}
