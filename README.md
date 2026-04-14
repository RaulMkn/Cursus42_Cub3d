*This project has been created as part of the 42 curriculum by ruortiz- and rmakende.*

# cub3D — My first RayCaster with miniLibX

## Description

cub3D is a 3D maze exploration project inspired by the classic Wolfenstein 3D game.
Using ray-casting principles and the miniLibX graphics library, the program renders a
first-person perspective view of a maze defined by a `.cub` scene file. The player can
navigate the maze in real time, with different textures displayed on each wall face
(North, South, East, West) and configurable floor and ceiling colors.

## Instructions

### Requirements

- Linux system with X11 support
- `cc` compiler (gcc/clang)
- X11 and Xext development libraries (`libx11-dev`, `libxext-dev`)

### Compilation

```bash
make
```

This will compile the miniLibX from sources, then the libft, and finally the cub3D binary.

### Execution

```bash
./cub3D maps/test.cub
```

The program takes a single argument: a scene description file with the `.cub` extension.

### Scene file format

The `.cub` file must contain the following elements (in any order except the map, which must be last):

```
NO ./path/to/north_texture.xpm
SO ./path/to/south_texture.xpm
WE ./path/to/west_texture.xpm
EA ./path/to/east_texture.xpm

F 220,100,0
C 50,50,200

111111
100N01
111111
```

- `NO`, `SO`, `WE`, `EA`: paths to `.xpm` texture files for each wall direction
- `F`: floor color as R,G,B values in range [0,255]
- `C`: ceiling color as R,G,B values in range [0,255]
- Map characters: `1` (wall), `0` (empty space), `N`/`S`/`E`/`W` (player start + orientation)
- The map must be closed/surrounded by walls

### Controls

| Key | Action |
|-----|--------|
| `W` | Move forward |
| `S` | Move backward |
| `A` | Strafe left |
| `D` | Strafe right |
| `←` | Rotate view left |
| `→` | Rotate view right |
| `ESC` | Quit |
| Window close button | Quit |

### Makefile rules

| Rule | Description |
|------|-------------|
| `make` / `make all` | Build the project |
| `make clean` | Remove object files |
| `make fclean` | Remove object files and binary |
| `make re` | Full rebuild |

## Resources

### Ray-casting references

- [Lode's Raycasting Tutorial](https://lodev.org/cgtutor/raycasting.html) — Main reference for the DDA ray-casting algorithm and texture mapping
- [miniLibX documentation](https://harm-smits.github.io/42docs/libs/minilibx) — Usage of mlx_hook, mlx_new_image, mlx_get_data_addr, etc.
- [Wolfenstein 3D source code](https://github.com/id-Software/wolf3d) — Historical reference
- [42 Docs: cub3D](https://harm-smits.github.io/42docs/projects/cub3d) — General project overview

### AI usage

AI tools (Antigravity by Google DeepMind and Claude) were used in this project for the following tasks:

- **Parser design**: Getting ideas on how to structure the `.cub` file parser into separate phases (load → header → map build → validate → transfer). The logic was reviewed, understood, and rewritten manually.
- **Norminette compliance checking**: Asking the AI to flag functions longer than 25 lines or with too many local variables, then fixing them manually.
- **Debugging & Fixing**: Describing specific execution scenarios causing issues (like map texture loading failures) and getting suggestions on where to look.
- **Project Structure**: Modifying Makefile compilation rules to correctly integrate the `minilibx-linux` library, generating compliant 42 File Headers dynamically, and managing realistic file metadata.
- **README structure**: The AI suggested the sections required by the subject; the content was written and verified by the student.

All AI-generated suggestions were critically reviewed, tested, and adapted. No code was copied blindly — every function in this project can be explained by the author.
