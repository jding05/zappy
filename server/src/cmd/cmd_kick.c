/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_kick.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 18:26:21 by sding             #+#    #+#             */
/*   Updated: 2018/08/17 18:26:24 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** use MAX_FD-> macro for max fd size
** it pushes them in the direction he is looking at
** When a client send to the server the kick command, all the
** clients in this square receive the following line:
**  moving <K>\n
** With K indicating the direction of the square where the player comes from
*/

/*
** kick a player from the square, return -> OK/KO
*/

#include "../../include/server.h"

static void	send_kick_msg(int fd, int direction)
{
	if (direction == NORTH)
	{
		printf(RED"player %d, moving SOUTH\n"RESET, fd);
		if (!send_data(fd, RED"moving SOUTH"RESET, MSG_SIZE))
			perror("Send [kick]");
	}
	else if (direction == EAST)
	{
		printf(RED"player %d, moving WEST\n"RESET, fd);
		if (!send_data(fd, RED"moving WEST"RESET, MSG_SIZE))
			perror("Send [kick]");
	}
	else if (direction == SOUTH)
	{
		printf(RED"player %d, moving NORTH\n"RESET, fd);
		if (!send_data(fd, RED"moving NORTH"RESET, MSG_SIZE))
			perror("Send [kick]");
	}
	else if (direction == WEST)
	{
		printf(RED"player %d, moving EAST\n"RESET, fd);
		if (!send_data(fd, RED"moving EAST"RESET, MSG_SIZE))
			perror("Send [kick]");
	}
}

/*
** # define NORTH 0
** # define EAST 1
** # define SOUTH 2
** # define WEST 3
**
** these functions are used in cmd_see.c
** use cmd_see find_cell_player()
*/

void		update_player_pos(int fd)
{
	int	y;
	int	x;

	y = g_players[fd].y;
	x = g_players[fd].x;
	if (y > g_env.map_y - 1)
		y = y - g_env.map_y;
	else if (y < 0)
		y = y + g_env.map_y;
	if (x > g_env.map_x - 1)
		x = x - g_env.map_x;
	else if (x < 0)
		x = x + g_env.map_x;
	g_players[fd].y = y;
	g_players[fd].x = x;
}

static int	find_cell_players(int y, int x, int direction, int fd)
{
	int	i;
	int	check;

	i = -1;
	check = 0;
	while (++i < MAX_FD)
	{
		if (g_players[i].y == y && g_players[i].x == x && i != fd &&
			!g_players[i].block && g_players[i].alive)
		{
			if (direction == NORTH)
				g_players[i].y -= 1;
			else if (direction == EAST)
				g_players[i].x += 1;
			else if (direction == SOUTH)
				g_players[i].y += 1;
			else if (direction == WEST)
				g_players[i].x -= 1;
			update_player_pos(i);
			send_kick_msg(i, direction);
			check = 1;
		}
	}
	return (check == 1 ? 1 : 0);
}

void		cmd_kick(int fd, char *msg, int player_id)
{
	(void)msg;
	printf(CYAN"\n[Exec KICK]\n"RESET);
	printf(BLUE"Player [%d] -> [%s]\n"RESET, fd, "kick");
	if (player_id != g_players[fd].player_id)
		return (error_return("player_id does not match"));
	g_players[fd].request_nb--;
	if (find_cell_players(g_players[fd].y, g_players[fd].x,
			g_players[fd].direction, fd))
		send_data(fd, RED"KICK OK"RESET, MSG_SIZE);
	else
	{
		send_data(fd, RED"KICK KO"RESET, MSG_SIZE);
		return ;
	}
	printf("players %d, pos-> y: %d x: %d d: %d\n", fd, g_players[fd].y,
			g_players[fd].x, g_players[fd].direction);
}
