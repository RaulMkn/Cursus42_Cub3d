/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 23:24:21 by ruortiz-          #+#    #+#             */
/*   Updated: 2026/04/08 23:24:22 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** PARSER.H
** =========
** Header exclusivo del parser. No mezcles esto con cub3d.h.
**
** CONTIENE:
** 1. Include de cub3d.h (para tener t_game) y libft.
** 2. La estructura t_parse: almacén temporal durante el parseo.
**    Una vez parseado todo, sus datos se transfieren a t_game y se libera.
** 3. Prototipos de todas las funciones del parser organizados por archivo.
**
** POR QUE t_parse SEPARADO DE t_game:
**    - t_game es la estructura del motor (render, mlx, etc.).
**    - t_parse es solo para el parser: guarda líneas del fichero,
**      estado intermedio, y datos antes de validarlos.
**    - Al final transfiere datos a t_game y se libera t_parse.
**
** CAMPOS DE t_parse:
**    lines       → array de strings con cada línea del .cub (raw)
**    line_count  → número total de líneas leídas
**    no/so/we/ea → rutas de texturas. NULL = aún no parseado.
**    floor_color / ceiling_color → -1 = aún no parseado.
**    map         → mapa normalizado (rectangular, relleno con ' ')
**    map_w/map_h → dimensiones del mapa normalizado
**    spawn_found → contador de spawns (debe ser exactamente 1 al final)
**    spawn_x/y   → posición de la celda de spawn en el mapa
**    spawn_dir   → 'N', 'S', 'E' o 'W'
*/

#ifndef PARSER_H
# define PARSER_H

# include "cub3d.h"
# include "libft.h"
# include "get_next_line.h"
# include <fcntl.h>

typedef struct s_parse
{
	char	**lines;
	int		line_count;
	char	*no;
	char	*so;
	char	*we;
	char	*ea;
	int		floor_color;
	int		ceiling_color;
	char	**map;
	int		map_w;
	int		map_h;
	int		spawn_found;
	int		spawn_x;
	int		spawn_y;
	char	spawn_dir;
}	t_parse;

/* --- parse_free.c --- */
void	free_lines(char **lines, int count);
void	free_parse(t_parse *p);
int		parse_error(t_parse *p, const char *msg);

/* --- parse_utils.c --- */
char	*skip_spaces(char *str);
void	trim_trailing(char *str);
int		visual_width(const char *s);
int		is_empty_line(const char *line);
int		str_is_digits(const char *str);

/* --- parse_file.c --- */
int		load_lines(const char *path, t_parse *p);

/* --- parse_header.c --- */
int		parse_header(t_parse *p);

/* --- parse_textures.c --- */
int		parse_texture_line(t_parse *p, const char *line, int id);

/* --- parse_colors.c --- */
int		parse_color_line(t_parse *p, const char *line, int id);

/* --- parse_map_build.c --- */
int		build_map(t_parse *p, int start_idx);

/* --- parse_map_validate.c --- */
int		validate_map(t_parse *p);

/* --- parse_player.c --- */
void	init_player(t_parse *p, t_game *game);

/* --- parse_main.c --- */
int		parse_cub_file(const char *path, t_game *game);

#endif
