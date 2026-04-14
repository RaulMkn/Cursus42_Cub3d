/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 00:00:00 by ruben             #+#    #+#             */
/*   Updated: 2026/04/08 10:18:46 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	free_lines(char **lines, int count)
{
	int	i;

	if (lines == NULL)
		return ;
	i = 0;
	while (i < count)
	{
		free(lines[i]);
		i++;
	}
	free(lines);
}

void	free_parse(t_parse *p)
{
	int	i;

	free_lines(p->lines, p->line_count);
	p->lines = NULL;
	free(p->no);
	free(p->so);
	free(p->we);
	free(p->ea);
	p->no = NULL;
	p->so = NULL;
	p->we = NULL;
	p->ea = NULL;
	if (p->map != NULL)
	{
		i = 0;
		while (p->map[i] != NULL)
		{
			free(p->map[i]);
			i++;
		}
		free(p->map);
		p->map = NULL;
	}
}

int	parse_error(t_parse *p, const char *msg)
{
	write(2, "Error\n", 6);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	free_parse(p);
	return (1);
}
