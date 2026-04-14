/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 00:00:00 by ruortiz-          #+#    #+#             */
/*   Updated: 2026/04/14 00:00:00 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define MENU_W		640
#define MENU_H		480
#define MAX_MAPS	32
#define ENTRY_H		52
#define ENTRY_X		80
#define ENTRY_W		480
#define MAPS_Y		180

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

static void	fill_rect(t_menu *m, int x, int y, int w, int h, int color)
{
	int	i;
	int	j;

	i = 0;
	while (i < h)
	{
		j = 0;
		while (j < w)
		{
			mlx_pixel_put(m->mlx, m->win, x + j, y + i, color);
			j++;
		}
		i++;
	}
}

static void	render_menu(t_menu *m)
{
	int		i;
	int		y;
	int		color;
	char	buf[64];

	mlx_clear_window(m->mlx, m->win);
	mlx_string_put(m->mlx, m->win, 200, 70, 0xFFFFFF,
		"CUB3D  -  Elige un mapa");
	mlx_string_put(m->mlx, m->win, 85, 115, 0x666666,
		"Flechas: navegar  |  Enter: jugar  |  ESC: salir");
	i = 0;
	while (i < m->count)
	{
		y = MAPS_Y + i * ENTRY_H;
		if (i == m->selected)
		{
			fill_rect(m, ENTRY_X, y - 8, ENTRY_W, ENTRY_H - 4, 0x1A3A5C);
			fill_rect(m, ENTRY_X, y - 8, 4, ENTRY_H - 4, 0x4A90D9);
			color = 0xFFFFFF;
		}
		else
			color = 0x888888;
		snprintf(buf, sizeof(buf), "%d.  %s", i + 1, m->names[i]);
		mlx_string_put(m->mlx, m->win, ENTRY_X + 16, y + 12, color, buf);
		i++;
	}
}

static int	menu_key_press(int key, t_menu *m)
{
	if (key == XK_Escape)
	{
		m->done = -1;
		mlx_loop_end(m->mlx);
	}
	else if (key == XK_Up || key == XK_k)
	{
		m->selected = (m->selected - 1 + m->count) % m->count;
		render_menu(m);
	}
	else if (key == XK_Down || key == XK_j)
	{
		m->selected = (m->selected + 1) % m->count;
		render_menu(m);
	}
	else if (key == XK_Return)
	{
		m->done = 1;
		mlx_loop_end(m->mlx);
	}
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
	m->win = NULL;
	mlx_loop_end(m->mlx);
	return (0);
}

static char	*strip_cub_ext(const char *name)
{
	const char	*dot;
	int			len;
	char		*out;

	dot = strrchr(name, '.');
	if (dot && strcmp(dot, ".cub") == 0)
		len = (int)(dot - name);
	else
		len = (int)strlen(name);
	out = malloc(len + 1);
	if (!out)
		return (NULL);
	memcpy(out, name, len);
	out[len] = '\0';
	return (out);
}

static void	scan_maps(t_menu *m)
{
	DIR				*dir;
	struct dirent	*entry;
	char			path[512];
	int				len;

	m->count = 0;
	dir = opendir("maps");
	if (!dir)
		return ;
	entry = readdir(dir);
	while (entry && m->count < MAX_MAPS)
	{
		len = strlen(entry->d_name);
		if (len > 4 && strcmp(entry->d_name + len - 4, ".cub") == 0)
		{
			snprintf(path, sizeof(path), "maps/%s", entry->d_name);
			m->maps[m->count] = strdup(path);
			m->names[m->count] = strip_cub_ext(entry->d_name);
			if (m->maps[m->count] && m->names[m->count])
				m->count++;
		}
		entry = readdir(dir);
	}
	closedir(dir);
}

static void	free_menu_data(t_menu *m)
{
	int	i;

	i = 0;
	while (i < m->count)
	{
		free(m->maps[i]);
		free(m->names[i]);
		i++;
	}
}

char	*show_map_menu(void)
{
	t_menu	m;
	char	*result;

	memset(&m, 0, sizeof(t_menu));
	scan_maps(&m);
	if (m.count == 0)
	{
		write(2, "Error\nNo se encontraron mapas .cub en maps/\n", 44);
		return (NULL);
	}
	m.mlx = mlx_init();
	if (!m.mlx)
		return (NULL);
	m.win = mlx_new_window(m.mlx, MENU_W, MENU_H, "CUB3D - Selecciona un mapa");
	if (!m.win)
	{
		mlx_destroy_display(m.mlx);
		free(m.mlx);
		free_menu_data(&m);
		return (NULL);
	}
	render_menu(&m);
	mlx_hook(m.win, KeyPress, 1L << 0, menu_key_press, &m);
	mlx_hook(m.win, Expose, 1L << 15, menu_expose, &m);
	mlx_hook(m.win, DestroyNotify, 0, menu_destroy, &m);
	mlx_loop(m.mlx);
	result = NULL;
	if (m.done == 1)
		result = strdup(m.maps[m.selected]);
	if (m.win)
		mlx_destroy_window(m.mlx, m.win);
	mlx_destroy_display(m.mlx);
	free(m.mlx);
	free_menu_data(&m);
	return (result);
}
