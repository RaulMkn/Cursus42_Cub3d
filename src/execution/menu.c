/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 00:54:03 by rmakende          #+#    #+#             */
/*   Updated: 2026/04/14 21:13:44 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"
#include <unistd.h>

static int	menu_key_press(int key, t_menu *m)
{
	if (key == XK_Escape)
	{
		m->done = -1;
		mlx_loop_end(m->mlx);
	}
	else if (key == XK_Up || key == XK_k)
		m->selected = (m->selected - 1 + m->count) % m->count;
	else if (key == XK_Down || key == XK_j)
		m->selected = (m->selected + 1) % m->count;
	else if (key == XK_Return)
	{
		m->done = 1;
		mlx_loop_end(m->mlx);
		return (0);
	}
	render_menu(m);
	return (0);
}

static int	menu_expose(t_menu *m)
{
	render_menu(m);
	return (0);
}

static int	menu_destroy(t_menu *m)
{
	m->done = -1;
	mlx_loop_end(m->mlx);
	return (0);
}

static int	init_menu(t_menu *m)
{
	m->mlx = mlx_init();
	if (!m->mlx)
		return (1);
	m->win = mlx_new_window(m->mlx, 640, 480, "CUB3D");
	if (!m->win)
	{
		mlx_destroy_display(m->mlx);
		free(m->mlx);
		m->mlx = NULL;
		return (1);
	}
	mlx_hook(m->win, 2, 1L << 0, menu_key_press, m);
	mlx_hook(m->win, 12, 1L << 15, menu_expose, m);
	mlx_hook(m->win, 17, 0, menu_destroy, m);
	return (0);
}

char	*show_map_menu(int argc, char **argv)
{
	t_menu	m;
	char	*r;

	ft_memset(&m, 0, sizeof(t_menu));
	scan_maps(&m, argc, argv);
	if (m.count == 0)
		return (write(2, "Error\nNo maps\n", 14), NULL);
	if (init_menu(&m) != 0)
		return (free_menu_data(&m), NULL);
	render_menu(&m);
	mlx_loop(m.mlx);
	r = NULL;
	if (m.done == 1)
		r = ft_strdup(m.maps[m.selected]);
	if (m.win)
		mlx_destroy_window(m.mlx, m.win);
	mlx_destroy_display(m.mlx);
	free(m.mlx);
	free_menu_data(&m);
	return (r);
}
