/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_broadcast.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 18:26:31 by sding             #+#    #+#             */
/*   Updated: 2018/08/17 18:26:33 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/server.h"

/*
** find the four position, base on the positon on the board
**     -> find 3 imagination spot, which might have neg value in either y or x
*/

void	calc_four_pos(int pos[4][2], int y, int x)
{
	if (y < g_env.map_y / 2)
	{
		pos[1][0] += g_env.map_y;
		pos[2][0] += g_env.map_y;
	}
	else if (y > g_env.map_y / 2)
	{
		pos[1][0] -= g_env.map_y;
		pos[2][0] -= g_env.map_y;
	}
	if (x < g_env.map_x / 2)
	{
		pos[2][1] += g_env.map_x;
		pos[3][1] += g_env.map_x;
	}
	else if (x > g_env.map_x / 2)
	{
		pos[2][1] -= g_env.map_x;
		pos[3][1] -= g_env.map_x;
	}
}

/*
** # define false 0 // need to check
*/

void	send_broadcast_msg(int nb_dir, int fd, char *msg)
{
	bzero(g_env.buffer, 4096);
	strcpy(g_env.buffer, "message ");
	strcat(g_env.buffer, ft_itoa(nb_dir));
	strcat(g_env.buffer, ",");
	strcat(g_env.buffer, msg);
	if (msg[strlen(msg) - 1] != '\n')
		strcat(g_env.buffer, "\n");
	if (send(fd, g_env.buffer, strlen(g_env.buffer), 0) == -1)
		perror("send [broadcast mssage]");
		// error(0, "send [broadcast mssage]", false);
}

/*
** we first assign 4 * 2 int 2D array with the value of current player position
**   then we called calc_four_pos function to
**      -> update 3 other spots that might go over the board boundary
**   later, we go thru every player to check if the player is the current player
**   if no -> 1. we calc the 4 possible paths to find the shortest destination
**            2. then call cal_direction function to find the square 1 - 8,
**                                      also update with direction
**            3. send to all the player the msg with square nb
*/

void	broadcast(int y, int x, int fd, char *msg)
{
	int i;
	int pos[4][2];
	int j;
	int nb_dir;

	i = -1;
	while (++i < 4)
	{
		pos[i][0] = y;
		pos[i][1] = x;
	}
	i = -1;
	calc_four_pos(pos, y, x);
	while (++i < MAX_FD)
	{
		if (i != fd)
		{
			j = get_closest_pos(pos, g_players[i].y, g_players[i].x);
			nb_dir = calc_direction(pos[j], g_players[i].y, g_players[i].x,\
									g_players[i].direction);
			send_broadcast_msg(nb_dir, i, msg);
		}
	}
}

/*
** all the players hear the broadcasts without knowing who emits.
**	-> they perceive only the direction the sound comes from and the message
**	the nb of the square crossed by the sound before it arrives to the player
**	indicate the direction. the number is done thru the attribution of "1"
**	to the square in front of the player, then count down of the squares
**	surrounding the player in the trigonometric direction (counter-clockwise)
**	the world is round therefore, we will choose the shortest tragectory for
**	the sound between the transmitter to the player for which we calculate
*/

int		cmd_broadcast(t_players players, char *msg)
{
	printf(BLUE"Player [%d] -> [%s <%s>]\n"RESET, players.fd, "broadcast", msg);
	players.request_nb--;
	broadcast(players.y, players.x, players.fd, msg);
	if (send_msg(players.fd, "OK", "Send [broadcast]") == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/*
** pos[4][2] is the four possible positions the current player at
** pos_y and pos_x is one of the other players in the game
**   -> compare 4 value to find the shortest one
*/

int		get_closest_pos(int pos[4][2], int pos_y, int pos_x)
{
	int		y;
	int		x;
	double	res[4];
	int		i;

	i = 0;
	while (i < 4)
	{
		y = (pos[i][0] - pos_y);
		x = (pos[i][1] - pos_x);
		res[i++] = sqrt(y * y + x * x);
	}
	if (res[0] < res[1] && res[0] < res[2] && res[0] < res[3])
		return (0);
	else if (res[1] < res[0] && res[1] < res[2] && res[1] < res[3])
		return (1);
	else if (res[2] < res[0] && res[2] < res[1] && res[2] < res[3])
		return (2);
	else if (res[3] < res[0] && res[3] < res[1] && res[3] < res[2])
		return (3);
	return (4);
}

/*
** pos[2] is the shortest path position from the four possible spot
** y, x, and direction here is one of other player's y, x, direction position
** to calculate:
**      calculate the y difference by y1 - y0, x difference by x1 - x0
**      diagolnal w/ slope -1 get the value by condition [ y1 - y0 = x1 - x0 ]
**      diagolnal w/ slope  1 get the value by cond [ y1 - y0 + (x1 - x0) = 0]
**      to differentiate the value on the right of the slope -1
**          -> if y gap < x gap
** update the square value based on the receiver player's direction
**  square nb example:
**  2 1 8    4 3 2    6 5 4   8 7 6
**  3 N 7    5 E 1    7 S 3   1 W 5
**  4 5 6    6 7 8    8 1 2   2 3 4
*/

int		calc_direction(int pos[2], int y, int x, int direction)
{
	int d;
	int y_diff;
	int x_diff;

	y_diff = pos[0] - y;
	x_diff = pos[1] - x;
	if (y_diff == x_diff)
		d = pos[1] < x ? 2 : 6;
	else if (y_diff + x_diff == 0)
		d = pos[1] < x ? 4 : 8;
	else if (y_diff > x_diff)
		d = abs(y_diff) > abs(x_diff) ? 1 : 7;
	else if (y_diff < x_diff)
		d = abs(y_diff) < abs(x_diff) ? 3 : 5;
	if (direction == NORTH)
		return (d);
	else if (direction == EAST)
		d = (d < 7) ? d + 2 : d - 6;
	else if (direction == SOUTH)
		d = (d < 5) ? d + 4 : d - 4;
	else if (direction == WEST)
		d = (d > 2) ? d - 2 : d + 6;
	return (d);
}
