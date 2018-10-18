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

#include "../../include/server.h"

void	cmd_incantation(int fd, char *msg, int player_id)
{
	int		i;
	int		count;
	int		fds[MAX_FD];
	int		level;
	int		nb;

	(void)msg;
	printf(CYAN"\n[Exec INCANTATION]\n"RESET);
	printf(BLUE"Player [%d] -> [%s]\n"RESET, fd, "incantation");
	if (player_id != g_players[fd].player_id)
		return (error_return("player_id does not match"));
	g_players[fd].request_nb--;
	i = -1;
	count = 0;
	level = g_players[fd].level;
	while (++i < MAX_FD)
	{
		if (g_players[i].y == g_players[fd].y &&
			g_players[i].x == g_players[fd].x && g_players[i].level == level &&
			check_prerequest(level, i) && g_players[i].block)
			fds[count++] = i;
	}
	if (count >= (nb = level_require(level)) && nb > 0)
		level_up_and_unblock(count, fds);
	printf(CYAN"\n[INCANTATION SUCCESS]\n"RESET);
}

void	level_up_and_unblock(int count, int fds[MAX_FD])
{
	int		i;
	char	*str;

	i = -1;
	bzero(g_env.buffer, MSG_SIZE);
	strcpy(g_env.buffer, RED"current level : "RESET);
	strcat(g_env.buffer, (str = ft_itoa(g_players[fds[0]].level + 1)));
	free(str);
	strcat(g_env.buffer, RESET);
	while (++i < count)
	{
		g_players[fds[i]].status = 0;
		if (g_players[fds[i]].level < 5)
			low_level_envolving_digest(g_players[fds[i]].level, fds[i]);
		else
			high_level_envolving_digest(g_players[fds[i]].level, fds[i]);
		g_players[fds[i]].level++;
		g_players[fds[i]].block = 0;
		if (g_players[fds[i]].level == 8)
			g_teams[g_players[fds[i]].team_id].reach_max_level++;
		send_data(fds[i], g_env.buffer, MSG_SIZE);
	}
}

/*
** check if players able to incantate
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

int		cmd_incantation_check(t_event *node)
{
	int		i;
	int		count;
	int		fds[MAX_FD];
	int		level;
	int		nb_players_require;

	i = -1;
	count = 0;
	level = g_players[node->fd].level;
	bzero(fds, sizeof(int) * MAX_FD);
	if (level >= 8)
		return (0);
	while (++i < MAX_FD)
	{
		if (g_players[i].y == g_players[node->fd].y && g_players[i].x ==
			g_players[node->fd].x && g_players[i].alive && !g_players[i].block)
			if (g_players[i].level == level && check_prerequest(level, i))
				fds[count++] = i;
	}
	nb_players_require = level_require(level);
	if (count >= nb_players_require)
		blocking(count, fds, node);
	else
		return (0);
	return (1);
}

void	blocking(int count, int fds[MAX_FD], t_event *node)
{
	int		i;

	i = -1;
	bzero(g_env.buffer, MSG_SIZE);
	strcpy(g_env.buffer, RED"elevation in progress"RESET);
	while (++i < count)
	{
		send_data(fds[i], g_env.buffer, MSG_SIZE);
		set_block_time(node, fds[i]);
		g_players[fds[i]].block = 1;
		g_players[fds[i]].status = 1;
	}
}
