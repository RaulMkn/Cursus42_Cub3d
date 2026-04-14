# RESUMEN COMPLETO DEL PARSER — cub3D

---

## Estructura de archivos

```
parsing/
  parser.h              → struct t_parse + todos los prototipos
  parse_free.c          → liberación de memoria + gestión de errores
  parse_utils.c         → helpers reutilizables (skip_spaces, etc.)
  parse_file.c          → carga del fichero .cub en memoria
  parse_header.c        → parseo de NO/SO/WE/EA/F/C
  parse_textures.c      → parseo y validación de rutas .xpm
  parse_colors.c        → parseo y validación de colores RGB
  parse_map_build.c     → construcción del mapa normalizado
  parse_map_validate.c  → validación de chars, spawn, cierre
  parse_player.c        → inicialización de posición y cámara
  parse_main.c          → orquestador + entry point
  RESUMEN_PARSER.md     → este archivo
```

---

## Flujo completo (orden de ejecución)

```
main(argv[1])
    └─ parse_cub_file(path, game)
           ├─ init_parse(&p)
           ├─ load_lines(path, &p)
           │      ├─ check_extension          → debe ser ".cub"
           │      ├─ count_file_lines         → dos pasadas sobre el fd
           │      └─ get_next_line x N        → llena p.lines[]
           │
           ├─ parse_header(&p)  → devuelve map_start_idx
           │      ├─ salta líneas vacías
           │      ├─ detecta identificador (get_header_id)
           │      ├─ parse_texture_line       → NO/SO/WE/EA
           │      ├─ parse_color_line         → F/C
           │      └─ all_headers_found check
           │
           ├─ build_map(&p, map_start_idx)
           │      ├─ find_map_width           → máximo ft_strlen de filas
           │      ├─ find_map_height          → cuenta filas, detecta vacía en medio
           │      └─ build_map_row x N        → copia + rellena con ' '
           │
           ├─ validate_map(&p)
           │      ├─ check_chars_and_spawn    → chars válidos + spawn único
           │      └─ check_map_closed         → cada '0'/spawn tiene 4 vecinos no-' '
           │
           ├─ init_player(&p, game)
           │      ├─ set_player_position      → spawn_x/y + 0.5
           │      ├─ set_player_direction     → dir/plane según N/S/E/W
           │      └─ map[spawn_y][spawn_x] = '0'
           │
           ├─ transfer_to_game(&p, game)      → mueve ownership, pone NULL en p
           └─ free_parse(&p)                  → libera solo lo temporal
```

---

## parser.h — struct t_parse

| Campo          | Tipo      | Valor inicial | Significado                          |
|----------------|-----------|---------------|--------------------------------------|
| `lines`        | `char **` | NULL          | Todas las líneas del .cub            |
| `line_count`   | `int`     | 0             | Número de líneas leídas              |
| `no/so/we/ea`  | `char *`  | NULL          | Rutas de texturas (NULL = no parseado) |
| `floor_color`  | `int`     | -1            | Color suelo 0xRRGGBB (-1 = no parseado) |
| `ceiling_color`| `int`     | -1            | Color techo 0xRRGGBB (-1 = no parseado) |
| `map`          | `char **` | NULL          | Mapa rectangular normalizado         |
| `map_w/map_h`  | `int`     | 0             | Dimensiones del mapa                 |
| `spawn_found`  | `int`     | 0             | Contador spawns (debe ser 1)         |
| `spawn_x/y`    | `int`     | 0             | Posición del spawn en el mapa        |
| `spawn_dir`    | `char`    | 0             | 'N', 'S', 'E' o 'W'                 |

---

## parse_free.c

| Función | Qué hace |
|---------|----------|
| `free_lines(lines, count)` | Libera cada `lines[i]` y luego el array |
| `free_parse(p)` | Libera todo en `t_parse`. Protegido contra NULL. Tras transfer_to_game los punteros están a NULL, así que no hay doble-free |
| `parse_error(p, msg)` | Escribe `"Error\n"` + msg en stderr, llama `free_parse`, devuelve `1` |

**Regla clave:** nunca uses `exit()` aquí. Devuelve `1` y el caller lo propaga.

---

## parse_utils.c

| Función | Qué hace |
|---------|----------|
| `skip_spaces(str)` | Devuelve puntero al primer no-espacio/tab. Sin copia. |
| `is_empty_line(line)` | 1 si NULL, vacía o solo espacios/tabs |
| `str_is_digits(str)` | 1 si todos los chars son dígitos (para validar RGB) |
| `realloc_lines(old, count)` | Reemplaza `realloc()` (prohibido por norma): crea array nuevo, copia, libera old |

---

## parse_file.c

| Función | Qué hace |
|---------|----------|
| `check_extension(path)` | Verifica que termine en `.cub` con `ft_strrchr` + `ft_strncmp` |
| `strip_newline(line)` | Quita el `\n` que deja `get_next_line` |
| `count_file_lines(path)` | Primera pasada: abre, cuenta, cierra |
| `load_lines(path, p)` | Segunda pasada: reserva `p->lines`, lee y guarda |

**Truco de dos pasadas:** abre el fd dos veces para no necesitar `realloc`.
Primera vez cuenta líneas, segunda vez las guarda.

---

## parse_textures.c

| Función | Qué hace |
|---------|----------|
| `get_texture_field(p, id)` | Devuelve `&p->no/so/we/ea` según id 0-3 |
| `extract_texture_path(line, prefix_len)` | Avanza prefix_len + skip_spaces, valida `.xpm`, devuelve `ft_strdup` |
| `parse_texture_line(p, line, id)` | Detecta duplicado, extrae path, asigna al campo |

**Validaciones:**
- Path vacío → error
- No termina en `.xpm` → error
- Campo ya != NULL → error (duplicado)

---

## parse_colors.c

| Función | Qué hace |
|---------|----------|
| `check_no_extra(str)` | Verifica que tras el número no haya basura |
| `parse_rgb_component(str, out)` | skip_spaces + str_is_digits + ft_atoi + rango [0,255] |
| `count_split(parts)` | Cuenta elementos del array hasta NULL |
| `parse_color_value(value_str, color_out)` | ft_split por ',', valida 3 partes, parsea r/g/b, calcula `(r<<16)\|(g<<8)\|b` |
| `parse_color_line(p, line, id)` | Detecta duplicado, avanza 1 char + skip_spaces, llama parse_color_value |

**Conversión:**
```
color = (r << 16) | (g << 8) | b   → formato 0xRRGGBB
```

---

## parse_header.c

| Función | Qué hace |
|---------|----------|
| `get_header_id(line)` | Compara con `ft_strncmp`: NO→0, SO→1, WE→2, EA→3, F→4, C→5, desconocido→-1 |
| `is_header_line(line)` | `get_header_id != -1` |
| `parse_one_header(p, line)` | Despacha a `parse_texture_line` o `parse_color_line` |
| `all_headers_found(p)` | Verifica que los 6 campos estén rellenos |
| `parse_header(p)` | **Devuelve el índice de inicio del mapa (int), no 0/1.** -1 en error |

**Detección de "F" vs "C":** solo reconoce `"F "` o `"F\t"` (con espacio).
Un carácter suelto `F` sin espacio → -1 (no es cabecera → inicio del mapa).

---

## parse_map_build.c

| Función | Qué hace |
|---------|----------|
| `find_map_width(p, start_idx)` | Máximo `ft_strlen` de todas las filas del mapa |
| `find_map_height(p, start_idx)` | Cuenta filas; si hay línea vacía en medio → error (-1) |
| `build_map_row(src, width)` | Copia src + rellena con `' '` hasta width + null-termina |
| `build_map(p, start_idx)` | Orquesta: calcula dimensiones, reserva `p->map`, construye filas |

**Por qué rellenar con `' '`:**
El motor trata `' '` como pared en `is_wall()`. Así las filas cortas no
causan lecturas fuera de rango.

---

## parse_map_validate.c

| Función | Qué hace |
|---------|----------|
| `valid_map_char(c)` | 1 si c ∈ {'0','1','N','S','E','W',' '} |
| `is_spawn_char(c)` | 1 si c ∈ {'N','S','E','W'} |
| `check_chars_and_spawn(p)` | Doble bucle: valida chars, cuenta spawns, guarda posición |
| `is_open_cell(p, x, y)` | 1 si algún vecino cardinal es `' '` o está fuera de límites |
| `check_map_closed(p)` | Para cada '0'/spawn, llama `is_open_cell` |
| `validate_map(p)` | Llama `check_chars_and_spawn` + `check_map_closed` |

**Por qué no flood-fill:**
La comprobación de 4 vecinos es suficiente con el mapa normalizado.
Si una celda transitable toca `' '` o el borde → mapa abierto.

---

## parse_player.c

| Función | Qué hace |
|---------|----------|
| `set_player_position(p, game)` | `player_x = spawn_x + 0.5`, `player_y = spawn_y + 0.5` |
| `set_player_direction(dir, game)` | Asigna dir_x/dir_y y plane_x/plane_y según N/S/E/W |
| `init_player(p, game)` | Llama las dos anteriores + convierte spawn a `'0'` en el mapa |

**Tabla de orientaciones:**

| Dir | dir_x | dir_y | plane_x | plane_y |
|-----|-------|-------|---------|---------|
| N   | 0     | -1    | 0.66    | 0       |
| S   | 0     | 1     | -0.66   | 0       |
| E   | 1     | 0     | 0       | 0.66    |
| W   | -1    | 0     | 0       | -0.66   |

**Crítico:** `map[spawn_y][spawn_x] = '0'` — el motor no entiende N/S/E/W.

---

## parse_main.c

| Función | Qué hace |
|---------|----------|
| `init_parse(p)` | Todo a NULL/0, colores a -1 |
| `transfer_to_game(p, game)` | Mueve map + texturas + colores a game, pone NULL en p, init mlx a NULL |
| `parse_cub_file(path, game)` | Orquesta las 8 fases en orden, propaga errores |

---

## Integración en main.c

```c
int main(int argc, char **argv)
{
    t_game  game;

    if (argc != 2)
        return (write(2, "Error\nUsage: ./cub3D map.cub\n", 29), 1);
    if (parse_cub_file(argv[1], &game) != 0)
        return (1);
    return (game_start(&game));
}
```

---

## Reglas norma 42 aplicadas

| Regla | Cómo se aplica |
|-------|----------------|
| Sin `exit()` interno | Solo `return (1)` propagado hasta `main` |
| Sin `realloc` | `realloc_lines()` en parse_utils.c |
| Sin `for` | Todos los bucles son `while` |
| Máx. 25 líneas/función | Funciones divididas (ej: validate_map tiene 4 subfunciones) |
| Máx. 5 variables locales | Respetado en todas las funciones |
| `Error\n` en stderr | `write(2, ...)` en `parse_error` |
| Sin leaks | Todo malloc tiene su free. free_parse libera en cualquier punto de fallo |

---

## Checklist antes de llamar game_start

- [ ] `game.map != NULL`
- [ ] `game.map_width > 0 && game.map_height > 0`
- [ ] `game.no_texture, so_texture, we_texture, ea_texture != NULL`
- [ ] `game.floor_color != -1 && game.ceiling_color != -1`
- [ ] `game.player_x/y` inicializados (spawn + 0.5)
- [ ] `game.dir_x/y` y `game.plane_x/y` inicializados
- [ ] La celda spawn en el mapa convertida a `'0'`
- [ ] `game.mlx_ptr == NULL && game.win_ptr == NULL`
- [ ] `game.win_width == WIN_WIDTH && game.win_height == WIN_HEIGHT`

---

## Casos de prueba mínimos (obligatorios en 42)

| # | Caso | Qué debe pasar |
|---|------|----------------|
| 1 | Fichero .cub válido pequeño | Parseo OK, juego arranca |
| 2 | Falta `NO` | `Error\nMissing one or more header identifiers` |
| 3 | Color fuera de rango `F 300,0,0` | `Error\nInvalid color format` |
| 4 | Dos spawns | `Error\nMap must have exactly one player spawn` |
| 5 | Sin spawn | `Error\nMap must have exactly one player spawn` |
| 6 | Mapa abierto por borde | `Error\nMap is not closed/surrounded by walls` |
| 7 | Carácter inválido en mapa | `Error\nInvalid character in map` |
| 8 | Ruta de textura no `.xpm` | `Error\nInvalid texture path or not .xpm` |
| 9 | Espacios múltiples `NO    ./a.xpm` | Parseo OK (skip_spaces los maneja) |
| 10 | Línea de cabecera después del mapa | `Error\nInvalid character in map` |
| 11 | Fichero con extensión incorrecta `.map` | `Error\nInvalid file extension` |
| 12 | Fichero vacío | `Error\nEmpty or unreadable file` |
| 13 | Identificador duplicado `NO` dos veces | `Error\nDuplicate texture identifier` |
| 14 | RGB con basura `F 1,2,3abc` | `Error\nInvalid color format` |
| 15 | Línea vacía en medio del mapa | `Error\nEmpty map or empty line inside map block` |
```
