/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 00:00:00 by ruortiz-          #+#    #+#             */
/*   Updated: 2026/04/14 00:00:00 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "../libft/libft.h"
#include <dirent.h>

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

	d = ft_strrchr(name, '.');
	if (d && ft_strncmp(d, ".cub", 5) == 0)
		len = (int)(d - name);
	else
		len = (int)ft_strlen(name);
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
		len = ft_strlen(e->d_name);
		if (len > 4 && ft_strncmp(e->d_name + len - 4, ".cub", 5) == 0)
		{
			ft_strlcpy(p, "maps/", sizeof(p));
			ft_strlcat(p, e->d_name, sizeof(p));
			m->maps[m->count] = ft_strdup(p);
			m->names[m->count] = strip_cub_ext(e->d_name);
			if (m->maps[m->count] && m->names[m->count])
				m->count++;
		}
		e = readdir(dir);
	}
	closedir(dir);
}
