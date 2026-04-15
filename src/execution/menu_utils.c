/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 00:00:00 by rmakende          #+#    #+#             */
/*   Updated: 2026/04/15 00:00:00 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"
#include "libft.h"
#include "get_next_line.h"

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

static void	try_add_entry(t_menu *m, char *line)
{
	char	p[512];
	int		len;

	len = (int)ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[--len] = '\0';
	if (len > 4 && ft_strncmp(line + len - 4, ".cub", 5) == 0
		&& (len - 4) <= MAX_NAME_LEN)
	{
		ft_strlcpy(p, "maps/", sizeof(p));
		ft_strlcat(p, line, sizeof(p));
		m->maps[m->count] = ft_strdup(p);
		m->names[m->count] = strip_cub_ext(line);
		if (m->maps[m->count] && m->names[m->count])
			m->count++;
	}
}

static void	scan_from_list(t_menu *m)
{
	int		fd;
	char	*line;

	fd = open("maps/maps.list", O_RDONLY);
	if (fd < 0)
		return ;
	line = get_next_line(fd);
	while (line && m->count < MAX_MENU_MAPS)
	{
		try_add_entry(m, line);
		free(line);
		line = get_next_line(fd);
	}
	if (line)
		free(line);
	close(fd);
}

void	scan_maps(t_menu *m, int argc, char **argv)
{
	(void)argc;
	(void)argv;
	m->count = 0;
	scan_from_list(m);
}
