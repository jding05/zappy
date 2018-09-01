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
** use FD_SETSIZE -> macro for max fd size
** it pushes them in the direction he is looking at
** When a client send to the server the kick command, all the
** clients in this square receive the following line:
**  moving <K>\n
** With K indicating the direction of the square where the player comes from
*/

/*
** kick a player from the square, return -> OK/KO
*/
#include <server.h>

int     cmd_kick(t_players players, char *msg)
{
    printf(BLUE"Player [$d] -> [%s]\n"RESET, players.fd, "kick");
    players.request_nb--;
    
    if (find_cell_player(players.y, players.x, players.direction))
        if (send(players.fd, "OK", 2, 0) == -1)
		    perror("Send [right]");
    else
    	if (send(players.fd, "KO", 2, 0) == -1)
		    perror("Send [right]");
    // maybe update graphic client regarding player position
	return (EXIT_SUCCESS);
}

void    send_kick_msg(t_player player, int direction)
{
    if (direction == NORTH)
        if (send(players.fd, "moving <NORTH>\n", 16, 0) == -1)
		    perror("Send [kick]");
    else if (direction == EAST)
        if (send(players.fd, "moving <EAST>\n", 15, 0) == -1)
		    perror("Send [kick]");
    else if (direction == SOUTH)
        if (send(players.fd, "moving <SOUTH>\n", 16, 0) == -1)
		    perror("Send [kick]");
    else if (direction == WEST)
        if (send(players.fd, "moving <WEST>\n", 15, 0) == -1)
		    perror("Send [kick]");
}
// # define NORTH 0
// # define EAST 1
// # define SOUTH 2
// # define WEST 3

// these functions are used in cmd_see.c
// use cmd_see find_cell_player()
static int find_cell_player(int y, int x, int direction)
{
    int i;
    int check;

    i = -1;
    check = 0;
    while (++i < FD_SETSIZE)
    {
        if (find_player_pos(y, x, i))
        {
            if (direction == NORTH)
                g_players[i].y -= 1;
            else if (direction == EAST)
                g_players[i].x += 1;
            else if (direction == SOUTH)
                g_players[i].y += 1;
            else if (direction == WEST)
                g_players[i].x -= 1;
            update_players_pos(g_players[i]);
            send_kick_msg(g_players[i], direction);
            check = 1;
        }
    }
    return (check == 1 ? 1 : 0);
}

static t_players    find_player_pos(int y, int x, int i)
{
    if (g_players[i].y == y && g_players[i].x == x)
        return (g_player[i]);
    return (NULL);
}

void    update_player_pos(t_players players)
{
    int y;
    int x;

    y = players.y;
    x = players.x;
    if (y > g_env.map_y -1)
        y = y - g_env.map_y;
    else if (y < 0)
        y = y + g_env.map_y;
    if (x > g_env.map_x -1)
        x = x - g_env.map_x;
    else if (x < 0)
        x = x + g_env.map_x;
    players.y = y;
    players.x = x;
}