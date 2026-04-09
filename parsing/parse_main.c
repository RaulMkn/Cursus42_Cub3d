/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 00:00:00 by ruben             #+#    #+#             */
/*   Updated: 2026/04/08 23:18:02 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** PARSE_MAIN.C — Orquestador: el ULTIMO archivo que implementas
**
** UNICO PUNTO DE ENTRADA PUBLICO:
**   main() solo llama a parse_cub_file(argv[1], &game).
**   Si devuelve 0, el juego puede arrancar. Si devuelve 1, termina.
**
** LAS 3 FUNCIONES Y SU LOGICA:
**
**   init_parse(p) → inicializa t_parse a valores seguros
**     Asigna campo a campo (no ft_bzero) porque:
**       ft_bzero pondria floor_color=0 y ceiling_color=0.
**       Pero 0 es negro puro (r=0,g=0,b=0), un color VALIDO.
**       Usamos -1 como centinela de "aun no parseado".
**       Despues del parseo, floor_color sera siempre >= 0.
**     Todos los punteros a NULL, todos los enteros a 0 (o -1 si
**     son centinelas), spawn_dir=0 (no '\0' para claridad con char).
**
**   transfer_to_game(p, game) → mueve ownership sin doble-free
**     Transfiere mapa, texturas y colores de t_parse a t_game.
**     PATRON para cada puntero:
**       game->map = p->map;   ← game hereda el bloque de memoria
**       p->map = NULL;        ← p deja de ser el "dueno"
**     Si NO pusieras p->map=NULL:
**       free_parse() liberaria p->map.
**       game->map apuntaria a memoria ya liberada → use-after-free.
**     Los colores (int) se copian por valor, sin patron NULL.
**     Tambien inicializa mlx_ptr=NULL, win_ptr=NULL, win_width/height.
**
**   parse_cub_file(path, game) ← funcion publica
**     Declara t_parse p en el STACK (no malloc), dura solo esta funcion.
**     Ejecuta las 8 fases en orden:
**       1. init_parse(&p)
**       2. load_lines(path, &p)       → error: return 1
**       3. map_start=parse_header(&p) → error: map_start<0, return 1
**       4. build_map(&p, map_start)   → error: return 1
**       5. validate_map(&p)           → error: return 1
**       6. init_player(&p, game)      → no puede fallar
**       7. transfer_to_game(&p, game) → no puede fallar
**       8. free_parse(&p)             → libera lo que quedo en p
**     return 0
**
** PROPAGACION DE ERRORES:
**   Cada fase llama a parse_error si falla, que ya llama free_parse.
**   Por eso en parse_cub_file simplemente hacemos return 1 sin limpiar:
**   la limpieza ya ocurrio dentro de la fase que fallo.
**
** EJEMPLO FLUJO COMPLETO (fichero valido):
**   init_parse: p todo a NULL/-1/0
**   load_lines: p->lines[0..6], p->line_count=7
**   parse_header: p->no/so/we/ea/floor/ceiling listos, return 4
**   build_map: p->map[0..3] normalizados, p->map[4]=NULL
**   validate_map: spawn en (3,1) dir='N', p->spawn_* listo
**   init_player: game->player_x=3.5 player_y=1.5 dir=(0,-1)
**   transfer_to_game: game->map=p->map p->map=NULL etc.
**   free_parse: libera p->lines (lo unico que queda en p)
**   return 0 → main llama game_start(&game)
*/

#include "parser.h"

/*
** init_parse — inicializa todos los campos de t_parse a valores seguros
**
** Parametro:
**   p → puntero al t_parse (en el stack, no lo liberes)
**
** Por que no usar ft_bzero aqui:
**   ft_bzero pondria floor_color y ceiling_color a 0.
**   Pero 0 es un color valido (negro puro: r=0, g=0, b=0).
**   Necesitamos -1 como centinela de "aun no parseado".
**   Asi que asignamos campo a campo.
**
** Pasos:
**   p->lines        = NULL
**   p->line_count   = 0
**   p->no           = NULL
**   p->so           = NULL
**   p->we           = NULL
**   p->ea           = NULL
**   p->floor_color  = -1     ← centinela: -1 significa "no parseado"
**   p->ceiling_color = -1    ← centinela: -1 significa "no parseado"
**   p->map          = NULL
**   p->map_w        = 0
**   p->map_h        = 0
**   p->spawn_found  = 0
**   p->spawn_x      = 0
**   p->spawn_y      = 0
**   p->spawn_dir    = 0      ← 0 (valor nulo), no '\0' por claridad con int
*/
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

/*
** transfer_to_game — mueve la memoria de t_parse a t_game
**
** Parametros:
**   p    → el t_parse con todos los datos ya validados
**   game → el t_game que recibirá los datos para el motor
**
** Devuelve: 0 siempre (no puede fallar si llegamos aqui)
**
** Pasos (mapa y dimensiones):
**   game->map        = p->map
**   p->map           = NULL         ← evita doble-free
**   game->map_width  = p->map_w
**   game->map_height = p->map_h
**
** Pasos (texturas, mismo patron para cada una):
**   game->no_texture = p->no
**   p->no            = NULL         ← evita doble-free
**   game->so_texture = p->so
**   p->so            = NULL
**   game->we_texture = p->we
**   p->we            = NULL
**   game->ea_texture = p->ea
**   p->ea            = NULL
**
** Pasos (colores, son int, no punteros, no hay doble-free):
**   game->floor_color   = p->floor_color
**   game->ceiling_color = p->ceiling_color
**
** Pasos (campos del motor que el parser debe inicializar):
**   game->mlx_ptr    = NULL         ← el motor lo inicializa en game_start
**   game->win_ptr    = NULL         ← el motor lo inicializa en game_start
**   game->win_width  = WIN_WIDTH    ← constante definida en cub3d.h
**   game->win_height = WIN_HEIGHT   ← constante definida en cub3d.h
**
** return (0)
*/
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

/*
** parse_cub_file — punto de entrada del parser, llamada desde main
**
** Parametros:
**   path → argv[1], la ruta al fichero .cub
**   game → puntero al t_game declarado en main (en el stack)
**
** Devuelve: 0 en exito, 1 en cualquier error
**
** Pasos:
**   1. Declara t_parse p (en el stack, no con malloc)
**      Declara int map_start
**
**   2. Llama a init_parse(&p)
**
**   3. Si load_lines(path, &p) devuelve distinto de 0:
**        return (1)
**        ← load_lines ya llamo a parse_error que llamo a free_parse
**
**   4. map_start = parse_header(&p)
**      Si map_start < 0:
**        return (1)
**        ← parse_header devuelve -1 en error (no 1)
**        ← parse_header ya llamo a parse_error que llamo a free_parse
**
**   5. Si build_map(&p, map_start) devuelve distinto de 0:
**        return (1)
**
**   6. Si validate_map(&p) devuelve distinto de 0:
**        return (1)
**
**   7. Llama a init_player(&p, game)
**        ← no puede fallar, no tiene return de error
**
**   8. Llama a transfer_to_game(&p, game)
**        ← mueve los punteros de p a game, pone NULL en p
**
**   9. Llama a free_parse(&p)
**        ← libera solo lo que quedaba en p (lineas del fichero)
**        ← los punteros transferidos ya son NULL, no se liberan
**
**  10. return (0)
**
** Integracion en main.c:
**
**   int main(int argc, char **argv)
**   {
**       t_game  game;
**
**       if (argc != 2)
**           return (write(2, "Error\nUsage: ./cub3D map.cub\n", 29), 1);
**       if (parse_cub_file(argv[1], &game) != 0)
**           return (1);
**       return (game_start(&game));
**   }
*/
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
