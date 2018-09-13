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

#include "../../include/server.h"

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
**
*/

void	send_broadcast_msg(int nb_dir, int fd, char *msg)
{
	bzero(g_env.buffer, MSG_SIZE);
	strcpy(g_env.buffer, RED"message ");
	strcat(g_env.buffer, ft_itoa(nb_dir));
	strcat(g_env.buffer, ",");
	strcat(g_env.buffer, msg);
	strcat(g_env.buffer, RESET);
	printf(RED"player %d, %s\n"RESET, fd, g_env.buffer);


	// if (send(fd, g_env.buffer, strlen(g_env.buffer), 0) == -1)
	// 	perror("send [broadcast mssage]");


	send_data(fd, g_env.buffer, MSG_SIZE);


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
	// printf("player %d, origin y %d, x %d\n", fd, y, x);
	calc_four_pos(pos, y, x);
	while (++i < MAX_FD)
	{
		if (i != fd && g_players[i].alive)
		{
			// printf("4 position: 1:{%d,%d}, 2:{%d,%d}, 3:{%d,%d}, 4:{%d,%d}\n",
			// pos[0][0],pos[0][1], pos[1][0],pos[1][1], pos[2][0],pos[2][1], pos[3][0],pos[3][1]);

			j = get_closest_pos(pos, g_players[i].y, g_players[i].x);
			nb_dir = calc_direction(pos[j], g_players[i].y, g_players[i].x,
									g_players[i].direction);
			// printf("pos %d -> , {%d,%d}are the closest\n, nb_dir %d\n", j + 1, pos[j][0], pos[j][1], nb_dir);
			// printf(RED"broadcast player %d, y: %d, x: %d, d: %d\n"RESET, fd, g_players[fd].y, g_players[fd].x, g_players[fd].direction);
			// printf(RED"got msg player %d, y: %d, x: %d, d: %d\n"RESET, i, g_players[i].y, g_players[i].x, g_players[i].direction);
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

int		cmd_broadcast(int fd, char *msg)
{
	printf(CYAN"\n[Exec BROADCAST]\n"RESET);
	printf(BLUE"Player [%d] -> [%s <%s>]\n"RESET, fd, "broadcast", msg);
	g_players[fd].request_nb--;
	broadcast(g_players[fd].y, g_players[fd].x, fd, msg);

	printf("players %d, pos-> y: %d x: %d d: %d\n", fd, g_players[fd].y, g_players[fd].x, g_players[fd].direction);
	printf(CYAN"\n[BROADCAST SUCCESS]\n"RESET);

	// if (send_msg(fd, "OK", "Send [broadcast]") == EXIT_FAILURE)
	// 	return (EXIT_FAILURE);

	send_data(fd, RED"BROADCAST OK"RESET, MSG_SIZE);

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
	// printf("4 distance: 1. %f, 2. %f, 3. %f, 4. %f\n", res[0], res[1], res[2], res[3]);
	if (res[0] < res[1] && res[0] < res[2] && res[0] < res[3])
		return (0);
	else if (res[1] < res[0] && res[1] < res[2] && res[1] < res[3])
		return (1);
	else if (res[2] < res[0] && res[2] < res[1] && res[2] < res[3])
		return (2);
	else if (res[3] < res[0] && res[3] < res[1] && res[3] < res[2])
		return (3);
	return (0);
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
**
** one edge case:
** -> when the other player is at the same position as current player
**     we will then send the other player the direction from 0
*/

int		calc_direction(int pos[2], int y, int x, int direction)
{
	int d;
	int y_diff;
	int x_diff;

	y_diff = pos[0] - y;
	x_diff = pos[1] - x;
	if (y_diff == 0 && x_diff == 0)
		return (0);
	else if (y_diff == x_diff)
		d = pos[1] < x ? 2 : 6;
	else if (y_diff + x_diff == 0)
		d = pos[1] < x ? 4 : 8;
	else if (y_diff > x_diff)
		d = abs(y_diff) > abs(x_diff) ? 5 : 3;
	else if (y_diff < x_diff)
		d = abs(y_diff) < abs(x_diff) ? 7 : 1;
	else // not sure for this case
		d = 1;
	printf("before adjust by got msg player nb_dir: %d\n", d);
	// if (direction == NORTH)
	// 	return (d);
	if (direction == EAST)
		d = (d < 7) ? d + 2 : d - 6;
	else if (direction == SOUTH)
		d = (d < 5) ? d + 4 : d - 4;
	else if (direction == WEST)
		d = (d > 2) ? d - 2 : d + 6;
	printf("before adjust by got msg player nb_dir: %d\n", d);
	return (d);
}
