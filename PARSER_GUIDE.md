# cub3D Parser Guide (ajustado a tu motor actual)

Este documento define como debe ser el parser para encajar con el codigo ya implementado en ejecucion/render.

## 1. Objetivo del parser

El parser debe dejar un `t_game` listo para llamar:

- `game_start(&game);`

Tu motor espera que estos campos de `t_game` ya esten correctamente inicializados:

- `map` (`char **`)
- `map_width`
- `map_height`
- `no_texture`
- `so_texture`
- `we_texture`
- `ea_texture`
- `floor_color`
- `ceiling_color`
- `player_x`
- `player_y`
- `dir_x`, `dir_y`
- `plane_x`, `plane_y`

Y tambien recomienda dejar en estado limpio:

- `mlx_ptr = NULL`
- `win_ptr = NULL`
- `win_width = WIN_WIDTH` (o un valor fijo propio)
- `win_height = WIN_HEIGHT` (o un valor fijo propio)

## 2. Contrato de integracion con tu motor

El motor actual usa:

- `is_wall()` para colisiones y DDA.
- `load_textures()` para cargar rutas `.xpm`.
- `render_frame()` para dibujar columnas.

Por tanto, el parser debe garantizar:

1. Mapa rectangular o indexable con seguridad.
2. Celdas fuera de mapa tratadas como pared (`' '`).
3. Un unico jugador convertido a posicion real en `player_x/player_y`.
4. El caracter de spawn (`N/S/E/W`) convertido a `'0'` al final del parse.
5. Texturas NO/SO/WE/EA existentes y validas.
6. Colores F/C convertidos a entero 0xRRGGBB.

## 3. Flujo recomendado del parser

1. Cargar archivo `.cub` completo en lineas.
2. Parsear cabecera (NO, SO, WE, EA, F, C).
3. Detectar inicio de bloque mapa.
4. Construir mapa normalizado (rectangular).
5. Validar mapa (chars, cierre, spawn unico).
6. Inicializar orientacion de camara en `t_game`.
7. Limpiar temporales y devolver exito.

## 4. Formato esperado de entrada

Cabecera:

- `NO path/to/no.xpm`
- `SO path/to/so.xpm`
- `WE path/to/we.xpm`
- `EA path/to/ea.xpm`
- `F r,g,b`
- `C r,g,b`

Mapa:

- Solo caracteres: `1`, `0`, `N`, `S`, `E`, `W`, espacio.
- Puede tener filas de longitudes distintas (normalizar internamente).

Reglas:

- Cada identificador de cabecera aparece exactamente una vez.
- No se permiten lineas de cabecera despues de comenzar el mapa.
- El mapa debe estar cerrado.

## 5. Estructuras sugeridas para el parser

Puedes usar una estructura intermedia y luego copiar a `t_game`:

```c
typedef struct s_parse
{
    char    **lines;
    int     line_count;

    char    *no;
    char    *so;
    char    *we;
    char    *ea;

    int     floor_color;
    int     ceiling_color;

    char    **map;
    int     map_w;
    int     map_h;

    int     spawn_found;
    int     spawn_x;
    int     spawn_y;
    char    spawn_dir;
}   t_parse;
```

## 6. Reglas de validacion criticas

### 6.1 Cabecera

- Duplicados => error.
- Falta de cualquier campo => error.
- Ruta vacia o solo espacios => error.
- Extension no `.xpm` => error.

### 6.2 Colores F/C

- Formato estricto: 3 enteros separados por coma.
- Rango por componente: `0..255`.
- Sin basura extra despues del tercer numero.

Conversion:

```c
color = (r << 16) | (g << 8) | b;
```

### 6.3 Mapa

- Caracter no permitido => error.
- Spawn distinto de 1 => error.
- `0` y spawn no pueden tocar borde abierto/espacio exterior.
- Filas vacias en medio del bloque mapa => error (salvo que tu diseno lo permita, pero suele causar mapas abiertos).

## 7. Normalizacion de mapa (importante para tu engine)

Como tu `is_wall()` considera `' '` como pared, conviene:

1. Calcular `map_width` como maximo largo de fila.
2. Reservar `map_height` filas de largo `map_width + 1`.
3. Copiar contenido original y rellenar resto con `' '`.
4. Null-terminar cada fila.

Esto evita lecturas fuera de rango y simplifica DDA.

## 8. Inicializacion del jugador/camara

Tras encontrar spawn en `(x, y)`:

- `player_x = x + 0.5`
- `player_y = y + 0.5`

Direccion y plano (FOV clasico ~66 deg):

- `N`: `dir=(0,-1)`, `plane=(0.66,0)`
- `S`: `dir=(0,1)`, `plane=(-0.66,0)`
- `E`: `dir=(1,0)`, `plane=(0,0.66)`
- `W`: `dir=(-1,0)`, `plane=(0,-0.66)`

Despues de guardar orientacion:

- `map[y][x] = '0'`

## 9. API minima sugerida

```c
int parse_cub_file(const char *path, t_game *game);
void free_parser_temp(t_parse *p);
int parse_error(t_parse *p, const char *msg);
```

Comportamiento recomendado:

- `parse_cub_file()` devuelve `0` en exito, `1` en error.
- En error, liberar todo lo temporal.
- En exito, transferir ownership de memoria a `t_game`.

## 10. Layout de archivos recomendado

```text
src/parsing/
  parse_main.c
  parse_file.c
  parse_header.c
  parse_textures.c
  parse_colors.c
  parse_map_build.c
  parse_map_validate.c
  parse_player.c
  parse_utils.c
  parse_free.c
include/
  parser.h
```

## 11. Manejo de errores (42)

Modelo simple:

- Imprimir `Error\n` y una descripcion corta.
- Nunca filtrar memoria en salidas por error.
- No salir con `exit()` desde funciones profundas; propagar error y limpiar arriba.

## 12. Checklist final antes de llamar game_start

- [ ] `map != NULL`
- [ ] `map_width > 0 && map_height > 0`
- [ ] 4 texturas asignadas
- [ ] `floor_color` y `ceiling_color` validos
- [ ] jugador inicializado (`player_x/y`, `dir`, `plane`)
- [ ] spawn convertido a `'0'`
- [ ] `mlx_ptr == NULL && win_ptr == NULL`
- [ ] `win_width/win_height` definidos

Si todo esta OK:

```c
if (parse_cub_file(argv[1], &game) != 0)
    return (1);
return (game_start(&game));
```

## 13. Casos de prueba minimos

1. Archivo valido pequeno.
2. Falta `NO`.
3. Color fuera de rango (`F 300,0,0`).
4. Dos spawns.
5. Sin spawn.
6. Mapa abierto por borde.
7. Mapa con caracter invalido.
8. Ruta de textura inexistente.
9. Espacios raros en cabecera (`NO    ./a.xpm`).
10. Linea de cabecera despues del mapa.

Con este contrato, tu parser quedara 100% alineado con el motor actual.
