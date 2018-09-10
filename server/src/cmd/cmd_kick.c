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

#include "../../inc/server.h"

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
		if (g_players[i].y == y && g_players[i].x == x && i != fd && !g_players[i].block && g_players[i].alive) // match player pos
		{
			// printf(RED"kick player %d, y: %d, x: %d, d: %d\n"RESET, fd, g_players[fd].y, g_players[fd].x, g_players[fd].direction);
			// printf(RED"move player %d, y: %d, x: %d, d: %d\n"RESET, i, g_players[i].y, g_players[i].x, g_players[i].direction);
			if (direction == NORTH)
				g_players[i].y -= 1;
			else if (direction == EAST)
				g_players[i].x += 1;
			else if (direction == SOUTH)
				g_players[i].y += 1;
			else if (direction == WEST)
				g_players[i].x -= 1;
			update_player_pos(i);
			// printf(RED"move player %d, y: %d, x: %d, d: %d\n"RESET, i, g_players[i].y, g_players[i].x, g_players[i].direction);
			send_kick_msg(i, direction);
			check = 1;
		}
	}
	return (check == 1 ? 1 : 0);
}

int			cmd_kick(int fd, char *msg)
{
	(void)msg;
	printf(CYAN"\n[Exec KICK]\n"RESET);
	printf(BLUE"Player [%d] -> [%s]\n"RESET, fd, "kick");
	g_players[fd].request_nb--;
	if (find_cell_players(g_players[fd].y, g_players[fd].x, g_players[fd].direction, fd))
	{
		if (send_msg(fd, RED"OK\n"RESET, "Send [kick]") == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else
	{
		if (send_msg(fd, RED"KO\n"RESET, "Send [kick]") == EXIT_FAILURE)
		return (EXIT_FAILURE);
	}
	printf("players %d, pos-> y: %d x: %d d: %d\n", fd, g_players[fd].y, g_players[fd].x, g_players[fd].direction);

	// maybe update graphic client regarding player position
	return (EXIT_SUCCESS);
}

/*
** if no colorize printing then, 15 or 16 bytes
*/

void		send_kick_msg(int fd, int direction)
{
	if (direction == NORTH)
	{
		printf(RED"player %d, moving <NORTH>\n"RESET, fd);
		if (send(fd, RED"moving <NORTH>\n"RESET, 26, 0) == -1)
			perror("Send [kick]");
	}
	else if (direction == EAST)
	{
		printf(RED"player %d, moving <EAST>\n"RESET, fd);
		if (send(fd, RED"moving <EAST>\n"RESET, 25, 0) == -1)
			perror("Send [kick]");
	}
	else if (direction == SOUTH)
	{
		printf(RED"player %d, moving <SOUTH>\n"RESET, fd);
		if (send(fd, RED"moving <SOUTH>\n"RESET, 26, 0) == -1)
			perror("Send [kick]");
	}
	else if (direction == WEST)
	{
		printf(RED"player %d, moving <WEST>\n"RESET, fd);
		if (send(fd, RED"moving <WEST>\n"RESET, 25, 0) == -1)
			perror("Send [kick]");
	}
}
