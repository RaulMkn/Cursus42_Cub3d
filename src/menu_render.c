/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 21:48:20 by rmakende          #+#    #+#             */
/*   Updated: 2026/04/14 21:54:13 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "../libft/libft.h"

#define ENTRY_H		52
#define ENTRY_X		80
#define ENTRY_W		480
#define MAPS_Y		180

static void	fill_rect(t_menu *m, int *r, int color)
{
	int	i;
	int	j;

	i = 0;
	while (i < r[3])
	{
		j = 0;
		while (j < r[2])
		{
			mlx_pixel_put(m->mlx, m->win, r[0] + j, r[1] + i, color);
			j++;
		}
		i++;
	}
}

static void	render_highlight(t_menu *m, int *r)
{
	r[2] = ENTRY_W;
	fill_rect(m, r, 0x1A3A5C);
	r[2] = 4;
	fill_rect(m, r, 0x4A90D9);
}

static char	*build_label(int idx, char *name)
{
	char	*num;
	char	*tmp;
	char	*label;

	num = ft_itoa(idx);
	tmp = ft_strjoin(num, ".  ");
	label = ft_strjoin(tmp, name);
	free(num);
	free(tmp);
	return (label);
}

static void	render_items(t_menu *m)
{
	int		i;
	int		y;
	int		color;
	int		r[4];
	char	*label;

	i = -1;
	while (++i < m->count)
	{
		y = MAPS_Y + i * ENTRY_H;
		r[0] = ENTRY_X;
		r[1] = y - 8;
		r[3] = ENTRY_H - 4;
		color = 0x888888;
		if (i == m->selected)
		{
			render_highlight(m, r);
			color = 0xFFFFFF;
		}
		label = build_label(i + 1, m->names[i]);
		mlx_string_put(m->mlx, m->win, ENTRY_X + 16, y + 12, color, label);
		free(label);
	}
}

void	render_menu(t_menu *m)
{
	mlx_clear_window(m->mlx, m->win);
	mlx_string_put(m->mlx, m->win, 200, 70, 0xFFFFFF,
		"cub3D  -  Elige un mapa");
	mlx_string_put(m->mlx, m->win, 85, 115, 0x666666,
		"Flechas: navegar  |  Enter: jugar  |  ESC: salir");
	render_items(m);
}
