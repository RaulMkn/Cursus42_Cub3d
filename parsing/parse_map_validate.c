/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_validate.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 00:00:00 by ruben             #+#    #+#             */
/*   Updated: 2026/04/08 14:38:24 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** PARSE_MAP_VALIDATE.C — Valida el mapa normalizado en 3 pasos
**
** POR QUE DESPUES DE BUILD:
**   Necesitamos el mapa rectangular y relleno con ' ' para poder
**   comprobar vecinos sin acceder fuera de los arrays.
**
** LAS 3 VALIDACIONES (en orden obligatorio):
**
**   1. CARACTERES VALIDOS + SPAWN UNICO  (check_chars_and_spawn)
**      Recorre todo el mapa. Por cada celda:
**        - Si el char no es '0','1','N','S','E','W',' ' → error.
**        - Si es N/S/E/W: register_spawn guarda x, y, dir y suma 1.
**      Al final: p->spawn_found debe ser exactamente 1.
**        0 spawns → el jugador no tiene donde aparecer → error.
**        2+ spawns → ambiguedad → error.
**      Ejemplo invalido: mapa con caracter '2' → error inmediato.
**
**   2. MAPA CERRADO  (check_map_closed + is_open_cell)
**      Por cada celda '0' o spawn llama is_open_cell(p, x, y):
**        - ¿Esta en el borde del array? (x=0, y=0, x=map_w-1...) → abierto
**        - ¿map[y-1][x]==' '? (vecino arriba es exterior)         → abierto
**        - ¿map[y+1][x]==' '? (vecino abajo es exterior)          → abierto
**        - ¿map[y][x-1]==' '? (vecino izquierda es exterior)      → abierto
**        - ¿map[y][x+1]==' '? (vecino derecha es exterior)        → abierto
**        - Si ninguno → cerrado (return 0)
**      No comprobamos si el vecino es '1'. Si es '0' u otro spawn,
**      ese otro caracter sera comprobado en su propia iteracion.
**
** EJEMPLO mapa cerrado (correcto):
**   111111
**   10N001    N en (2,1): arriba='1' abajo='0' izq='0' der='0' OK
**   100001    0 en (1,2): arriba='0' abajo='1' izq='1' der='0' OK
**   111111
**
** EJEMPLO mapa abierto (incorrecto):
**   111111
**   10N  1    0 en (2,1): vecino derecha=' ' → is_open_cell=1 → error
**   100001
**   111111
**
** ORDEN IMPORTA:
**   Primero check_chars_and_spawn: si hay chars invalidos, is_open_cell
**   podria comportarse mal con caracteres inesperados.
**   Segundo check_map_closed: usa spawn data guardada por el primer check.
**
** RELACION CON parse_player.c (el siguiente):
**   Tras validate_map, p->spawn_x, p->spawn_y, p->spawn_dir estan listos.
**   init_player los usa para calcular la posicion y camara del jugador.
*/

#include "parser.h"

/*
** valid_map_char — comprueba si un caracter es valido en el mapa
**
** Parametro:
**   c → el caracter a comprobar (char, comillas simples en comparaciones)
**
** Devuelve: 1 si es valido, 0 si no lo es
**
** Pasos (todo en un return con ||):
**   return (c == '0' || c == '1' || c == 'N'
**        || c == 'S' || c == 'E' || c == 'W' || c == ' ')
**
** Cada comparacion usa comillas simples porque son chars individuales.
** NO uses == "0" (string) para comparar un char.
*/
static void	register_spawn(t_parse *p, int x, int y, char c)
{
	p->spawn_found++;
	p->spawn_x = x;
	p->spawn_y = y;
	p->spawn_dir = c;
}

/*
** check_chars_and_spawn — valida todos los chars y encuentra el spawn
**
** Parametro:
**   p → el struct t_parse (con p->map ya construido)
**
** Devuelve: 0 en exito, 1 en error
**
** Pasos:
**   1. i = 0
**   2. Bucle while i < p->map_h:
**        j = 0
**        Bucle while j < p->map_w:
**          c = p->map[i][j]
**
**          Si !valid_map_char(c):
**            return (parse_error(p, "Invalid character in map"))
**
**          Si is_spawn_char(c):
**            p->spawn_found++
**            p->spawn_x = j
**            p->spawn_y = i
**            p->spawn_dir = c
**
**          j++
**        i++
**
**   3. Si p->spawn_found != 1:
**        return (parse_error(p, "Map must have exactly one spawn"))
**        - 0 spawns → el jugador no tiene donde aparecer
**        - 2+ spawns → ambiguedad, error de diseno del mapa
**
**   4. return (0)
**
** Por que guardamos spawn_x, spawn_y, spawn_dir:
**   Los usaremos despues en init_player para calcular la posicion
**   y orientacion del jugador.
*/
static int	check_chars_and_spawn(t_parse *p)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	while (i < p->map_h)
	{
		j = 0;
		while (j < p->map_w)
		{
			c = p->map[i][j];
			if (!(c == '0' || c == '1' || c == 'N'
					|| c == 'S' || c == 'E' || c == 'W' || c == ' '))
				return (parse_error(p, "Invalid character in map"));
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
				register_spawn(p, j, i, c);
			j++;
		}
		i++;
	}
	if (p->spawn_found != 1)
		return (parse_error(p, "Map must have exactly one spawn"));
	return (0);
}

/*
** is_open_cell — comprueba si una celda transitable esta expuesta
**
** Parametros:
**   p → el struct t_parse
**   x → columna de la celda a comprobar
**   y → fila de la celda a comprobar
**
** Devuelve: 1 si la celda esta abierta (problema), 0 si esta cerrada
**
** Pasos:
**   1. Si x <= 0 O y <= 0 O x >= p->map_w - 1 O y >= p->map_h - 1:
**        return (1)
**        ← la celda esta en el borde del array → mapa abierto
**        ← una celda transitable en el borde no puede estar rodeada de paredes
**
**   2. Si p->map[y - 1][x] == ' ' O p->map[y + 1][x] == ' ':
**        return (1)
**        ← vecino de arriba o abajo es espacio exterior
**        ← ' ' con comillas simples = char espacio
**
**   3. Si p->map[y][x - 1] == ' ' O p->map[y][x + 1] == ' ':
**        return (1)
**        ← vecino de izquierda o derecha es espacio exterior
**
**   4. return (0)   ← los 4 vecinos son paredes ('1'), correcto
**
** Por que no miramos si el vecino es '1' explicitamente:
**   Solo comprobamos si es ' ' (espacio, exterior).
**   Si es '0' u otro spawn, sera valido si ESE caracter tambien
**   tiene sus vecinos correctos (se comprueba en su propia iteracion).
*/
static int	is_open_cell(t_parse *p, int x, int y)
{
	if (x <= 0 || y <= 0 || x >= p->map_w - 1 || y >= p->map_h - 1)
		return (1);
	if (p->map[y - 1][x] == ' ' || p->map[y + 1][x] == ' ')
		return (1);
	if (p->map[y][x - 1] == ' ' || p->map[y][x + 1] == ' ')
		return (1);
	return (0);
}

/*
** check_map_closed — verifica que todo el mapa esta cerrado
**
** Parametro:
**   p → el struct t_parse
**
** Devuelve: 0 si el mapa esta cerrado, 1 si hay alguna celda abierta
**
** Pasos:
**   1. i = 0
**   2. Bucle while i < p->map_h:
**        j = 0
**        Bucle while j < p->map_w:
**          c = p->map[i][j]
**
**          Si (c == '0' O is_spawn_char(c)) Y is_open_cell(p, j, i):
**            return (parse_error(p, "Map is not closed/surrounded by walls"))
**
**          j++
**        i++
**
**   3. return (0)
**
** Por que solo comprobamos '0' y spawn (no '1'):
**   Las celdas de pared ('1') no son accesibles por el jugador.
**   Solo importa que las celdas TRANSITABLES esten cerradas.
**   Las paredes pueden estar en el borde del array sin problema.
*/
static int	check_map_closed(t_parse *p)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	while (i < p->map_h)
	{
		j = 0;
		while (j < p->map_w)
		{
			c = p->map[i][j];
			if ((c == '0' || c == 'N' || c == 'S'
					|| c == 'E' || c == 'W') && is_open_cell(p, j, i))
				return (parse_error(p, "Map is not closed"));
			j++;
		}
		i++;
	}
	return (0);
}

/*
** validate_map — funcion publica: orquesta las dos validaciones
**
** Parametro:
**   p → el struct t_parse
**
** Devuelve: 0 si el mapa es valido, 1 si hay cualquier error
**
** Pasos:
**   1. Si check_chars_and_spawn(p) devuelve distinto de 0:
**        return (1)
**        ← el error ya fue impreso por check_chars_and_spawn
**
**   2. Si check_map_closed(p) devuelve distinto de 0:
**        return (1)
**        ← el error ya fue impreso por check_map_closed
**
**   3. return (0)
**
** Orden importa: primero validamos chars y spawn, porque is_open_cell
** usa is_spawn_char que asume que solo hay chars validos en el mapa.
*/
int	validate_map(t_parse *p)
{
	if (check_chars_and_spawn(p) != 0)
		return (1);
	if (check_map_closed(p) != 0)
		return (1);
	return (0);
}
