/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_incantation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 18:26:40 by sding             #+#    #+#             */
/*   Updated: 2018/08/17 18:26:41 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/server.h"

int		cmd_incantation(t_players players, char *msg)
{
	int		i;
	int		count;
	int		fds[100];
	int		level;

	(void)msg;
	printf(BLUE"Player [%d] -> [%s]\n"RESET, players.fd, "incantation");
	players.request_nb--;
	i = -1;
	count = 0;
	level = players.level;
	while (++i < MAX_FD)
	{
		if (g_players[i].y == players.y && g_players[i].x == players.x)
			if (g_players[i].level == level && check_prerequest(level, i))
				fds[count++] = i;
	}
	if (count >= level_require(level))
		level_up_and_unblock(count, fds);
	//	maybe update graphic client regarding player position
	return (EXIT_SUCCESS);
}

void	level_up_and_unblock(int count, int fds[100])
{
	int		i;

	i = -1;
	while (++i < count)
	{
		g_players[fds[i]].level++;
		g_players[fds[i]].block = 0;
		if (g_players[fds[i]].level == 8)
			g_teams[g_players[fds[i]].team_id].reach_max_level++;
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

// from cmd_see.c or cmd_kick.c
/*
** get the current player's position input
**    to see if there if other player in the same position (cell)
**    if yes -> check if other player's level match the current player's level
**              if yes -> store the player descriptor into int array fds[];
**                        also update how many player meet the level requirement
**    get the nb players require to incantate by level_require function
**    -> check if nb players in the same cell same level meet the requirement nb
**          if yes -> level up all the player descriptor stored in fds[]
**             no -> return 0 -> means the incantation command failed
*/

/*
** change -> check if players able to incantate
*/

int		cmd_incantation_check(t_players players, char *msg)
{
	int		i;
	int		count;
	int		fds[100];
	int		level;

	(void)msg;
	i = -1;
	count = 0;
	level = players.level;
	while (++i < MAX_FD)
	{
		if (g_players[i].y == players.y && g_players[i].x == players.x)
			if (g_players[i].level == level && check_prerequest(level, i))
				fds[count++] = i;
	}
	if (count >= level_require(level))
		blocking(count, fds);
	else
		return (0);
	return (1);
}

void	blocking(int count, int fds[100])
{
	int		i;
	char	buffer[41];

	i = -1;
	strcpy(buffer, "elevation in progress\ncurrent level : k\n");
	while (++i < count)
	{
		buffer[38] = g_players[i].level + '0';
		if (send(i, buffer, 41, 0) == -1)
			perror("Send [incantation]");
		g_players[fds[i]].block = 1;
	}
}
