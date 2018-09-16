/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_incantation_util.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/15 22:07:41 by sding             #+#    #+#             */
/*   Updated: 2018/09/15 22:07:42 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.h"

void	low_level_envolving_digest(int level, int fd)
{
	if (level == 1)
		g_players[fd].inventory[1]--;
	else if (level == 2)
	{
		g_players[fd].inventory[1] -= 1;
		g_players[fd].inventory[2] -= 1;
		g_players[fd].inventory[3] -= 1;
	}
	else if (level == 3)
	{
		g_players[fd].inventory[1] -= 2;
		g_players[fd].inventory[3] -= 1;
		g_players[fd].inventory[5] -= 2;
	}
	else if (level == 4)
	{
		g_players[fd].inventory[1] -= 1;
		g_players[fd].inventory[2] -= 1;
		g_players[fd].inventory[3] -= 2;
		g_players[fd].inventory[5] -= 1;
	}
}

void	high_level_envolving_digest(int level, int fd)
{
	if (level == 5)
	{
		g_players[fd].inventory[1] -= 1;
		g_players[fd].inventory[2] -= 2;
		g_players[fd].inventory[3] -= 1;
		g_players[fd].inventory[4] -= 3;
	}
	else if (level == 6)
	{
		g_players[fd].inventory[1] -= 1;
		g_players[fd].inventory[2] -= 2;
		g_players[fd].inventory[3] -= 3;
		g_players[fd].inventory[5] -= 1;
	}
	else if (level == 7)
	{
		g_players[fd].inventory[1] -= 2;
		g_players[fd].inventory[2] -= 2;
		g_players[fd].inventory[3] -= 2;
		g_players[fd].inventory[4] -= 2;
		g_players[fd].inventory[5] -= 2;
		g_players[fd].inventory[6] -= 1;
	}
}

int		check_prerequest(int level, int i)
{
	int		*inv;

	inv = g_players[i].inventory;
	if (level == 1)
		return (inv[1] > 0 ? 1 : 0);
	else if (level == 2)
		return ((inv[1] > 0 && inv[2] > 0 && inv[3] > 0) ? 1 : 0);
	else if (level == 3)
		return ((inv[1] > 1 && inv[3] > 0 && inv[5] > 1) ? 1 : 0);
	else if (level == 4)
		return ((inv[1] > 0 && inv[2] > 0 && inv[3] > 1 && inv[5] > 0) ? 1 : 0);
	else if (level == 5)
		return ((inv[1] > 0 && inv[2] > 1 && inv[3] > 0 && inv[4] > 2) ? 1 : 0);
	else if (level == 6)
		return ((inv[1] > 0 && inv[2] > 1 && inv[3] > 2 && inv[5] > 0) ? 1 : 0);
	else if (level == 7)
		return ((inv[1] > 1 && inv[2] > 1 && inv[3] > 1 && inv[4] > 1 && \
		inv[5] > 1 && inv[6] > 0) ? 1 : 0);
	return (0);
}

/*
** get the input "level" and return the nb players needed
**      for the incantation
*/

int		level_require(int level)
{
	if (level == 1)
		return (1);
	else if (level == 2 || level == 3)
		return (2);
	else if (level == 4 || level == 5)
		return (4);
	else if (level == 6 || level == 7)
		return (6);
	else
		return (0);
}
