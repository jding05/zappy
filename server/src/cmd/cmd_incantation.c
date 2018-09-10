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

int		cmd_incantation(int fd, char *msg)
{
	int		i;
	int		count;
	int		fds[100];
	int		level;
	int		nb;

	(void)msg;
	printf(CYAN"\n[Exec INCANTATION]\n"RESET);
	printf(BLUE"Player [%d] -> [%s]\n"RESET, fd, "incantation");
	printf("players %d, level %d\n", fd, g_players[fd].level);
	g_players[fd].request_nb--;
	i = -1;
	count = 0;
	level = g_players[fd].level;
	while (++i < MAX_FD)
	{
		if (g_players[i].y == g_players[fd].y && g_players[i].x == g_players[fd].x)
			if (g_players[i].level == level && check_prerequest(level, i))
				fds[count++] = i;
	}
	if (count >= (nb = level_require(level)) && nb > 0)
		level_up_and_unblock(count, fds);
	//	maybe update graphic client regarding player position
	printf(CYAN"\n[INCANTATION SUCCESS]\n"RESET);
	printf("players %d, level %d\n", fd, g_players[fd].level);
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

int		cmd_incantation_check(int fd) // need to norm
{
	int		i;
	int		count;
	int		fds[100];
	int		level;
	int		nb_players_require;

	i = -1;
	count = 0;
	level = g_players[fd].level;
	// printf("player: |%d|, level: |%d|\n", fd, level);
	bzero(fds, sizeof(int) * 100);
	if (level >= 8)
	{
		send_msg(fd, RED"KO\n"RESET, "Send [incantation]");
		// printf("Player level: %d > 8\n",level);
		return (0);
	}
	while (++i < MAX_FD)
	{
		if (g_players[i].y == g_players[fd].y && g_players[i].x == g_players[fd].x &&
			g_players[i].alive && !g_players[i].block)
		{
			if (g_players[i].level == level && check_prerequest(level, i))
				fds[count++] = i;
		}
	}
	// printf(RED"\n[level %d]\n"RESET, level);
	nb_players_require = level_require(level);
	// printf("count: |%d|, nb_players_require: |%d|\n", count, nb_players_require);
	if (count >= nb_players_require)
		blocking(count, fds);
	else
		return (0);
	return (1);
}

void	blocking(int count, int fds[100])
{
	int		i;

	i = -1;
	bzero(g_env.buffer, 4096);
	strcpy(g_env.buffer, RED"elevation in progress\ncurrent level : k\n"RESET);
	while (++i < count)
	{
		g_env.buffer[38] = g_players[i].level + '0';
		send_msg(i, g_env.buffer, "Send [incantation]");
		g_players[fds[i]].block = 1;
	}
}
