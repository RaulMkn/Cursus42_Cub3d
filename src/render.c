#include "cub3d.h"

static void	draw_ceiling_floor(t_game *game, int x, t_ray *ray)
{
	int	y;

	y = 0;
	while (y < ray->draw_start)
	{
		put_pixel(&game->screen, x, y, game->ceiling_color);
		y++;
	}
	y = ray->draw_end + 1;
	while (y < game->win_height)
	{
		put_pixel(&game->screen, x, y, game->floor_color);
		y++;
	}
}

static void	draw_textured_wall(t_game *game, int x, t_ray *ray)
{
	int		y;
	int		tex_y;
	int		color;
	t_img	*tex;

	tex = &game->tex[ray->tex_index];
	y = ray->draw_start;
	while (y <= ray->draw_end)
	{
		tex_y = (int)ray->tex_pos;
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= tex->height)
			tex_y = tex->height - 1;
		color = get_tex_pixel(tex, ray->tex_x, tex_y);
		put_pixel(&game->screen, x, y, color);
		ray->tex_pos += ray->tex_step;
		y++;
	}
}

void	render_frame(t_game *game)
{
	int		x;
	t_ray	ray;

	x = 0;
	while (x < game->win_width)
	{
		cast_one_ray(game, &ray, x);
		draw_ceiling_floor(game, x, &ray);
		draw_textured_wall(game, x, &ray);
		x++;
	}
}
