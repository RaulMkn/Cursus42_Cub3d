/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 00:00:00 by ruben             #+#    #+#             */
/*   Updated: 2026/04/08 11:56:21 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static char	**get_texture_field(t_parse *p, int id)
{
	if (id == 0)
		return (&p->no);
	else if (id == 1)
		return (&p->so);
	else if (id == 2)
		return (&p->we);
	else if (id == 3)
		return (&p->ea);
	else
		return (NULL);
}

static char	*extract_texture_path(const char *line, int prefix_len)
{
	char	*path;
	char	*dot;

	path = skip_spaces((char *)(line + prefix_len));
	if (!path || path[0] == '\0')
		return (NULL);
	dot = ft_strrchr(path, '.');
	if (!dot)
		return (NULL);
	if (ft_strncmp(dot, ".xpm", 5) != 0)
		return (NULL);
	return (ft_strdup(path));
}

int	parse_texture_line(t_parse *p, const char *line, int id)
{
	char	**field;
	char	*path;

	field = get_texture_field(p, id);
	if (!field)
		return (parse_error(p, "Unknown texture identifier"));
	if (*field)
		return (parse_error(p, "Duplicate texture identifier"));
	path = extract_texture_path(line, 2);
	if (!path)
		return (parse_error(p, "Invalid texture path or not .xpm"));
	*field = path;
	return (0);
}
