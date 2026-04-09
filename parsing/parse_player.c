/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_player.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 00:00:00 by ruben             #+#    #+#             */
/*   Updated: 2026/04/08 23:12:04 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** PARSE_PLAYER.C — Convierte el spawn en estado inicial del jugador
**
** QUE RECIBE (de validate_map):
**   p->spawn_x   → columna del spawn en el mapa (int, ej: 3)
**   p->spawn_y   → fila del spawn en el mapa (int, ej: 1)
**   p->spawn_dir → char: 'N', 'S', 'E' o 'W'
**
** QUE PRODUCE (en t_game):
**   game->player_x / player_y → posicion continua centrada en la celda
**   game->dir_x / dir_y       → vector de direccion de la camara
**   game->plane_x / plane_y   → plano de camara (define el FOV)
**   p->map[spawn_y][spawn_x]  → se convierte a '0' (limpia el spawn)
**
** LAS 4 FUNCIONES Y SU LOGICA:
**
**   set_player_position(p, game)
**     game->player_x = p->spawn_x + 0.5
**     game->player_y = p->spawn_y + 0.5
**     POR QUE +0.5: el motor usa coordenadas continuas. Cada celda
**     ocupa 1.0 unidad. Sin +0.5 el jugador estaria en la esquina
**     de la celda, lo que puede causar colisiones erroneas al inicio.
**     Ejemplo: spawn en (3,1) → player_x=3.5, player_y=1.5
**
**   set_dir_ns(dir, game) / set_dir_ew(dir, game)
**     Asignan dir y plane segun la direccion. Separados en 2 funciones
**     para cumplir el limite de 25 lineas de la norma.
**     POR QUE ESTOS VALORES:
**       dir y plane deben ser PERPENDICULARES (requisito matematico).
**       plane_x o plane_y = 0.66 define un FOV de ~66 grados.
**       En el sistema de coordenadas del motor, Y crece hacia abajo:
**         Norte → dir_y=-1 (y decrece, vas hacia arriba del mapa)
**         Sur   → dir_y=+1 (y crece, vas hacia abajo del mapa)
**
**   Tabla de vectores (FOV ~66 grados):
**     Dir | dir_x | dir_y | plane_x | plane_y
**      N  |   0   |  -1   |  +0.66  |    0
**      S  |   0   |  +1   |  -0.66  |    0
**      E  |  +1   |   0   |    0    |  +0.66
**      W  |  -1   |   0   |    0    |  -0.66
**
**   set_player_direction(dir, game)
**     Wrapper: si N o S llama set_dir_ns, si E o W llama set_dir_ew.
**
**   init_player(p, game) ← funcion publica, no puede fallar
**     1. set_player_position
**     2. set_player_direction
**     3. p->map[spawn_y][spawn_x] = '0'
**        Si no limpiamos el spawn, is_wall() vera 'N' como pared
**        y el jugador quedara atrapado en el primer frame.
**
** EJEMPLO completo con spawn 'N' en (3,1):
**   player_x = 3.5, player_y = 1.5
**   dir_x=0, dir_y=-1, plane_x=0.66, plane_y=0
**   map[1][3]: 'N' → '0'
**
** RELACION CON parse_main.c (el siguiente):
**   init_player no transfiere nada a t_game por si solo: escribe
**   directamente en game->player_x/y, dir, plane.
**   Despues, transfer_to_game mueve map, texturas y colores de t_parse
**   a t_game, completando la inicializacion antes de game_start.
*/

#include "parser.h"

/*
** set_player_position — asigna la posicion inicial del jugador
**
** Parametros:
**   p    → contiene spawn_x y spawn_y (coordenadas enteras del mapa)
**   game → donde guardamos player_x y player_y (doubles)
**
** Pasos:
**   1. game->player_x = p->spawn_x + 0.5
**      → 0.5 es un literal double, el resultado es double automaticamente
**
**   2. game->player_y = p->spawn_y + 0.5
*/
static void	set_player_position(t_parse *p, t_game *game)
{
	game->player_x = p->spawn_x + 0.5;
	game->player_y = p->spawn_y + 0.5;
}

/*
** set_player_direction — asigna direccion y plano de camara segun spawn_dir
**
** Parametros:
**   dir  → el caracter de spawn: 'N', 'S', 'E' o 'W' (comillas simples)
**   game → donde guardamos dir_x, dir_y, plane_x, plane_y
**
** Pasos (usa if/else if, sin switch):
**
**   Si dir == 'N':          ← mira hacia arriba (y decrece)
**     game->dir_x   = 0
**     game->dir_y   = -1
**     game->plane_x = 0.66
**     game->plane_y = 0
**
**   Si no, si dir == 'S':   ← mira hacia abajo (y crece)
**     game->dir_x   = 0
**     game->dir_y   = 1
**     game->plane_x = -0.66
**     game->plane_y = 0
**
**   Si no, si dir == 'E':   ← mira hacia la derecha (x crece)
**     game->dir_x   = 1
**     game->dir_y   = 0
**     game->plane_x = 0
**     game->plane_y = 0.66
**
**   Si no, si dir == 'W':   ← mira hacia la izquierda (x decrece)
**     game->dir_x   = -1
**     game->dir_y   = 0
**     game->plane_x = 0
**     game->plane_y = -0.66
**
** Los valores 0.66 y -0.66 del plano definen el FOV de ~66 grados.
** dir y plane son siempre perpendiculares: es un requisito matematico
** del algoritmo de raycasting.
*/
static void	set_dir_ns(char dir, t_game *game)
{
	game->dir_x = 0;
	game->plane_y = 0;
	if (dir == 'N')
	{
		game->dir_y = -1;
		game->plane_x = 0.66;
	}
	else
	{
		game->dir_y = 1;
		game->plane_x = -0.66;
	}
}

static void	set_dir_ew(char dir, t_game *game)
{
	game->dir_y = 0;
	game->plane_x = 0;
	if (dir == 'E')
	{
		game->dir_x = 1;
		game->plane_y = 0.66;
	}
	else
	{
		game->dir_x = -1;
		game->plane_y = -0.66;
	}
}

static void	set_player_direction(char dir, t_game *game)
{
	if (dir == 'N' || dir == 'S')
		set_dir_ns(dir, game);
	else
		set_dir_ew(dir, game);
}

/*
** init_player — inicializa todo lo relativo al jugador
**
** Parametros:
**   p    → el struct t_parse (tiene spawn_x, spawn_y, spawn_dir, map)
**   game → el struct t_game (donde guardamos la info del jugador)
**
** Pasos:
**   1. Llama a set_player_position(p, game)
**
**   2. Llama a set_player_direction(p->spawn_dir, game)
**        → p->spawn_dir es el char guardado por check_chars_and_spawn
**
**   3. p->map[p->spawn_y][p->spawn_x] = '0'
**        → Convierte la celda del spawn en suelo transitable
**        → '0' en comillas simples = char cero (no el numero 0)
**        → Esta modificacion afecta a p->map, que luego se
**          transfiere a game->map en transfer_to_game
*/
void	init_player(t_parse *p, t_game *game)
{
	set_player_position(p, game);
	set_player_direction(p->spawn_dir, game);
	p->map[p->spawn_y][p->spawn_x] = '0';
}
