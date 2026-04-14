/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 00:00:00 by ruben             #+#    #+#             */
/*   Updated: 2026/04/08 10:40:26 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	*skip_spaces(char *str)
{
	while (*str == ' ' || *str == '\t' || *str == '\n')
		str++;
	return (str);
}

void	trim_trailing(char *str)
{
	int	len;

	len = (int)ft_strlen(str);
	while (len > 0
		&& (str[len - 1] == ' ' || str[len - 1] == '\t'))
		str[--len] = '\0';
}

int	is_empty_line(const char *line)
{
	if (!line)
		return (1);
	while (*line == ' ' || *line == '\t' || *line == '\n')
		line++;
	return (*line == '\0');
}

int	str_is_digits(const char *str)
{
	if (!str || str[0] == '\0')
		return (0);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

char	**realloc_lines(char **old, int old_count)
{
	char	**new_arr;
	int		i;

	new_arr = malloc(sizeof(char *) * (old_count + 1));
	if (!new_arr)
		return (NULL);
	i = 0;
	while (i < old_count)
	{
		new_arr[i] = old[i];
		i++;
	}
	free(old);
	return (new_arr);
}
