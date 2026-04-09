/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_colors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 00:00:00 by ruben             #+#    #+#             */
/*   Updated: 2026/04/08 14:08:36 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	check_no_extra(const char *str)
{
	int	i;

	i = 0;
	while (ft_isdigit(str[i]))
		i++;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] != '\0')
		return (1);
	return (0);
}

static int	parse_rgb_component(const char *str, int *out)
{
	char	*s;
	int		val;
	int		flag;

	s = skip_spaces((char *)str);
	if (!str_is_digits(s))
		return (1);
	flag = 0;
	val = ft_atoi(s, &flag);
	if (val < 0 || val > 255)
		return (1);
	*out = val;
	return (0);
}

static int	count_split(char **parts)
{
	int	i;

	i = 0;
	while (parts[i])
		i++;
	return (i);
}

static int	parse_color_value(const char *value_str, int *color_out)
{
	char	**parts;
	int		r;
	int		g;
	int		b;

	parts = ft_split(value_str, ',');
	if (!parts || count_split(parts) != 3)
	{
		ft_free_split(parts);
		return (1);
	}
	if (parse_rgb_component(parts[0], &r)
		|| parse_rgb_component(parts[1], &g)
		|| parse_rgb_component(parts[2], &b)
		|| check_no_extra(skip_spaces((char *)parts[2])))
	{
		ft_free_split(parts);
		return (1);
	}
	ft_free_split(parts);
	*color_out = (r << 16) | (g << 8) | b;
	return (0);
}

int	parse_color_line(t_parse *p, const char *line, int id)
{
	char	*value;
	int		color;

	if (id == 4 && p->floor_color != -1)
		return (parse_error(p, "Duplicate F identifier"));
	if (id == 5 && p->ceiling_color != -1)
		return (parse_error(p, "Duplicate C identifier"));
	value = skip_spaces((char *)(line + 1));
	if (!value || value[0] == '\0')
		return (parse_error(p, "Empty color value"));
	if (parse_color_value(value, &color) != 0)
		return (parse_error(p, "Invalid color format"));
	if (id == 4)
		p->floor_color = color;
	else
		p->ceiling_color = color;
	return (0);
}
