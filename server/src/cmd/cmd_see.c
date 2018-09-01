/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_see.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/30 15:56:58 by sding             #+#    #+#             */
/*   Updated: 2018/08/30 15:57:02 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server.h>


void        print_player_info(t_players players)
{
    printf(" [player %d] ", players.fd);
    // printf("team: %d\n", players.team_id);
    // printf("level: %d\n", players.level);
    // printf("dead: %d\n", players.dead);
    // printf("direction: %d\n", players.direction);
    // printf("pos y: %d\n", players.y);
    // printf("pos x: %d\n", players.x);
    // printf("request_nb: %d\n", players.request_nb);
    // printf("-----------------------------\n");
}

void        find_cell_player(int y, int x)
{
    int i;

    i = -1;
    while (++i < FD_SETSIZE)
        if (find_player_pos(y, x, i))
            print_player_info(g_player[i]);
}

/*
** if the current player's pos cell has other players 
** -> return other player
*/
t_players    find_player_pos(int y, int x, int i)
{
    if (g_players[i].y == y && g_players[i].x == x)
        return (g_player[i]);
    return (NULL);
}

/*
** print_resource() needs to change print to send, maybe save all the things to buffer
*/
void    print_resource(int y, int x)
{
    int     i;
    char    *resource[] = {"food", "linemate", "deraumere", \
                "sibur", "mendiane", "phiras", "thystame"};

    i = 0;
    while (i < 7)
    {
        // printf(LIME" %d, "RESET, map[y][x][i++]);
        if (map[y][x][i] > 0)
        {
            printf(LIME"%s:"RESET, resource[i]);
            printf(LIME" %d "RESET, map[y][x][i]);
        }
        i++;
    }
    find_cell_player(y, x);
    printf(", ");
}

/*
** update the cell (y,x) coordinate
** -> due to the coordinate might over the board
** -> then call the print_resource function
**      -> to print the resource if exist, and player according to assign cell
*/
void    print_cell_value(int y, int x)
{
    // printf("v: |%d|, ", map[y][x]);
    // printf("y: |%d|, x: |%d|", y, x);
    if (y > g_env.map_y - 1)
        y = y - g_env.map_y;
    else if (y < 0)
        y = y + g_env.map_y;
    if (x > MAP_X -1)
        x = x - g_env.map_x;
    else if (x < 0)
        x = x + g_env.map_x;
    // printf(LIME" %d, "RESET, map[y][x]);
    // printf("y: |%d|, x: |%d|\n", y, x);
    print_resource(y, x);
}

/*
** see north area from the (y,x) coordinate, expand with the level
*/
void    see_north_area(int level, int y, int x)
{
    // int count;
    int floor;
    int x_end;
    int x_start;

    // count = 0;
    floor = 0;
    x_end = x + 1;
    printf("{");
    while (floor < level + 1)
    {
        x_start = x - floor; // 5 - 0 = 5
        // printf(UCYAN"\n[floor: %d]\n"RESET, floor);
        while (x_start < x_end)
        {
            print_cell_value(y, x_start);

            // count++;
            // if (count == (level + 1) * (level + 1))
            //     return ;
            x_start++;
        }
        floor++;
        y--;
        x_end++;
    }
    printf("}");
}

/*
** see south area from the (y,x) coordinate, expand with the level
*/
void    see_south_area(int level, int y, int x)
{
    int floor;
    int x_end;
    int x_start;

    floor = 0;
    x_end = x - 1;
    printf("{");
    while (floor <= level)
    {
        x_start = x + floor; // 5 - 0 = 5
        // printf(UCYAN"\n[floor: %d]\n"RESET, floor);
        while (x_start > x_end)
        {
            print_cell_value(y, x_start);
            x_start--;
        }
        floor++;
        y++;
        x_end--;
    }
    printf("}");
}

/*
** see east area from the (y,x) coordinate, expand with the level
*/
void    see_east_area(int level, int y, int x)
{
    int floor;
    int y_end;
    int y_start;

    floor = 0;
    y_end = y + 1;
    printf("{");
    while (floor <= level)
    {
        y_start = y - floor;
        // printf(UCYAN"\n[floor: %d]\n"RESET, floor);
        while (y_start < y_end)
        {
            print_cell_value(y_start, x);
            y_start++;
        }
        floor++;
        x++;
        y_end++;
    }
    printf("}");
}

/*
** see west area from the (y,x) coordinate, expand with the level
*/
void    see_west_area(int level, int y, int x)
{
    int floor;
    int y_end;
    int y_start;

    floor = 0;
    y_end = y - 1;
    printf("{");
    while (floor <= level)
    {
        y_start = y + floor; // 5 - 0 = 5
        // printf(UCYAN"\n[floor: %d]\n"RESET, floor);
        while (y_start > y_end)
        {
            print_cell_value(y_start, x);
            y_start--;
        }
        floor++;
        x--;
        y_end--;
    }
    printf("}");
}

int		cmd_see(t_players players, char *msg)
{
    char    *connect_nbr;
    t_team  *team;

    printf(BLUE"Player [$d] -> [%s]\n"RESET, players.fd, "see");
    players.request_nb--;
    if (players.direction == NORTH)
        see_north_area(players.level, players.y, players.x);
    else if (players.direction == SOUTH)
        see_south_area(players.level, players.y, players.x);
    else if (players.direction == WEST)
        see_south_area(players.level, players.y, players.x);
    else if (players.direction == EAST)
        see_south_area(players.level, players.y, players.x);
	// maybe update graphic client regarding player position
	return (EXIT_SUCCESS);
}