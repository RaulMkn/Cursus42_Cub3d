/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_player.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruortiz- <ruortiz-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 00:00:00 by ruortiz-          #+#    #+#             */
/*   Updated: 2026/04/13 00:00:00 by ruortiz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	update_player(t_game *game)
{
	if (game->keys.w)
		move_forward_backward(game, MOVE_SPEED);
	if (game->keys.s)
		move_forward_backward(game, -MOVE_SPEED);
	if (game->keys.a)
		move_strafe(game, MOVE_SPEED);
	if (game->keys.d)
		move_strafe(game, -MOVE_SPEED);
	if (game->keys.left)
		rotate_player(game, -ROT_SPEED);
	if (game->keys.right)
		rotate_player(game, ROT_SPEED);
}
