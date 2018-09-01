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

#include <server.h>

int		cmd_incantation(t_players players, char *msg)
{
	printf(BLUE"Player [$d] -> [%s]\n"RESET, players.fd, "incantation");
	players.request_nb--;
    if (find_cell_player(players.y, players.x, players.level))
        if (send(players.fd, "OK", 2, 0) == -1)
		    perror("Send [incantation]");
    // maybe update graphic client regarding player position
    return (EXIT_SUCCESS);
}

/*
** check if the players get level up reach the max level 8
**    if yes -> update that team's reach_max_level count 
*/
void    update_team_max_level(int level, int team_id)
{
    t_team *team;

    team = g_env.team;
    while (team)
    {
        if (team->team_number == team_id)
            break;
        team = team->next;
    }
    team->reach_max_level++;
}

/*
** level up the players which stored in fds by find_cell_players function
**    and send the Response message to those player to tell them
**    which level they are right now after the incantation
*/
void    level_up(int count, int fds[])
{
    int     i;
    char    buffer[41];

    i = 0;
    buffer = "elevation in progress\ncurrent level : k\n";
    while (i < count)
    {
        g_players[i].level++;
        buffer[38] = g_players[i].level + '0';
        if (send(players.fd, buffer, 41, 0) == -1)
		    perror("Send [incantation]");
        if (g_players[i].level == 8)
            update_team_max_level(g_players[i].level, g_players[i].team_id);
        i++;
    }
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
int     find_cell_player(int y, int x, int level)
{
    int         i;
    t_player    player;
    int         count;
    int         fds[100];

    i = -1;
    count = 0;
    while (++i < FD_SETSIZE)
    {
        if (player = find_player_pos(y, x, i))
            if (check_level(level, i))
                fds[count++] = i;
    }
    if (count >= level_require(level))
        level_up(count, fds);
    else
        return (0);
    return (1);
}

/*
** get the input "level" and return the nb players needed
**      for the incantation
*/
int    level_require(int level)
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

/*
** find if other player's position match the current players
** if yes -> return the match player struct
**    no  -> return NULL
*/
static t_players    find_player_pos(int y, int x, int i)
{
    if (g_players[i].y == y && g_players[i].x == x)
        return (g_player[i]);
    return (NULL);
}

/*
** check if the player meet the require level to incantate
*/
int    check_level(int level, int fd)
{
    return(g_players[fd].level == level ? 1 : 0);
}