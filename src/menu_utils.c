/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 01:09:53 by rmakende          #+#    #+#             */
/*   Updated: 2026/04/14 21:13:31 by rmakende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "../libft/libft.h"
#include <dirent.h>
#include <string.h>
#include <stdio.h>

void	free_menu_data(t_menu *m)
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

static char	*strip_cub_ext(const char *name)
{
	const char	*d;
	int			len;
	char		*out;

	d = strrchr(name, '.');
	if (d && strcmp(d, ".cub") == 0)
		len = (int)(d - name);
	else
		len = (int)strlen(name);
	out = malloc(len + 1);
	if (!out)
		return (NULL);
	ft_memcpy(out, name, len);
	out[len] = '\0';
	return (out);
}

void	scan_maps(t_menu *m)
{
	DIR				*dir;
	struct dirent	*e;
	char			p[512];
	int				len;

	m->count = 0;
	dir = opendir("maps");
	if (!dir)
		return ;
	e = readdir(dir);
	while (e && m->count < MAX_MAPS)
	{
		len = strlen(e->d_name);
		if (len > 4 && strcmp(e->d_name + len - 4, ".cub") == 0)
		{
			snprintf(p, sizeof(p), "maps/%s", e->d_name);
			m->maps[m->count] = ft_strdup(p);
			m->names[m->count] = strip_cub_ext(e->d_name);
			if (m->maps[m->count] && m->names[m->count])
				m->count++;
		}
		e = readdir(dir);
	}
	closedir(dir);
}
