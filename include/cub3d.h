/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 00:00:00 by ruortiz-          #+#    #+#             */
/*   Updated: 2026/04/13 00:00:00 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdlib.h>
# include <math.h>
# include "mlx.h"
# include <X11/X.h>
# include <X11/keysym.h>

# define WIN_WIDTH 1024
# define WIN_HEIGHT 768
# define TEX_NO 0
# define TEX_SO 1
# define TEX_WE 2
# define TEX_EA 3
# define MOVE_SPEED 0.08
# define ROT_SPEED 0.05
# define MAX_MAPS 32

typedef struct s_menu
{
	void	*mlx;
	void	*win;
	char	*maps[MAX_MAPS];
	char	*names[MAX_MAPS];
	int		count;
	int		selected;
	int		done;
}	t_menu;

typedef struct s_img
{
	void	*img_ptr;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		width;
	int		height;
}	t_img;

typedef struct s_keys
{
	int	w;
	int	s;
	int	a;
	int	d;
	int	left;
	int	right;
}	t_keys;

typedef struct s_ray
{
	double	camera_x;
	double	ray_dir_x;
	double	ray_dir_y;
	int		map_x;
	int		map_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	side_dist_x;
	double	side_dist_y;
	double	perp_dist;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
	int		line_height;
	int		draw_start;
	int		draw_end;
	double	wall_x;
	int		tex_x;
	double	tex_step;
	double	tex_pos;
	int		tex_index;
}	t_ray;

typedef struct s_game
{
	char	**map;
	int		map_width;
	int		map_height;
	char	*no_texture;
	char	*so_texture;
	char	*we_texture;
	char	*ea_texture;
	int		floor_color;
	int		ceiling_color;
	double	player_x;
	double	player_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	void	*mlx_ptr;
	void	*win_ptr;
	int		win_width;
	int		win_height;
	t_img	screen;
	t_img	tex[4];
	t_keys	keys;
}	t_game;

int		game_start(t_game *game);
int		game_loop(t_game *game);
int		close_game(t_game *game);
int		on_destroy(t_game *game);
int		on_key_press(int keycode, t_game *game);
int		on_key_release(int keycode, t_game *game);

int		init_mlx(t_game *game);
int		init_window(t_game *game);
int		init_screen(t_game *game);
int		load_textures(t_game *game);

void	render_frame(t_game *game);
void	cast_one_ray(t_game *game, t_ray *ray, int x);
void	ray_set_start(t_game *game, t_ray *ray, int x);
void	ray_set_projection(t_game *game, t_ray *ray);
void	ray_set_sampling(t_game *game, t_ray *ray);

void	update_player(t_game *game);
void	move_forward_backward(t_game *game, double speed);
void	move_strafe(t_game *game, double speed);
void	rotate_player(t_game *game, double angle);

int		is_wall(t_game *game, double x, double y);
void	put_pixel(t_img *img, int x, int y, int color);
int		get_tex_pixel(t_img *img, int x, int y);

void	destroy_textures(t_game *game);
void	destroy_screen(t_game *game);
void	free_game_data(t_game *game);

char	*show_map_menu(void);
void	render_menu(t_menu *m);
void	scan_maps(t_menu *m);
void	free_menu_data(t_menu *m);

#endif
