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
** Header exclusivo del parser. No mezcles esto con cub3d.h.
** CONTIENE:
** 1. Include e structs.
** 2. t_parse para datos intermedios.
** 3. Prototipos de funciones.
** CAMPOS DE t_parse documentados en el codigo.
*/

#ifndef PARSER_H
# define PARSER_H

# include "../include/cub3d.h"
# include "../libft/libft.h"
# include "../libft/get_next_line.h"
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
int		is_empty_line(const char *line);
int		str_is_digits(const char *str);
char	**realloc_lines(char **old, int old_count);

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
