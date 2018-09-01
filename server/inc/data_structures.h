#ifndef DATA_STRUCTURES_H
# define DATA_STRUCTURES_H

/*
 *world width and height - stored in t_map map
 * team names - stored in team_stats
 * clients authorized -
 *
*/

# include "server.h"

// typedef struct      s_egg
// {

// }                   t_egg;

typedef struct      s_team
{
    int             team_number;
    char            *team_name;
    int             max_players;        // sding add it
    int             connected_players;  // sding added it
    int             reach_max_level;    // sding added it
    // t_egg           egg; // need to added it
    struct s_team   *next;
}                   t_team;

// typedef struct      s_client // sding delete it
// {
//     int             socket;
//     t_team          *team_stats;
// }                   t_client;

typedef struct      s_players
{
    // t_client        *client; // sding delete it 
    // t_team          *team;
    int             team_id;
    // int             food;
    int             level;
    int             dead;       // sding add it , to update and delete
	int				fd;
	int				direction;	// sding added it --> 0: N, 1: E, 2: S, 3: W
	int				y;			// sding added it
	int				x;			// sding added it
	int				request_nb; // sding added it
	int				inventory[7]; // sding added it
}                   t_players;

/* might not use this struct, but leave it here for resource reference
typedef struct      s_cell
{
    // int                 food         0
    // int                 linemate;    1
    // int                 deraumere;   2
    // int                 sibur;       3
    // int                 mendiane;    4
    // int                 phiras;      5
    // int                 thystame;    6
	int			    resource[7];    
	// t_players	**players; // sding delete it 
}                       t_cell;
*/

/* replace it with the int 3D array
*****************************
typedef struct      s_map
{
    int             x;
    int             y;
    t_cell   **cell;
}                   t_map;
*/

/*
** note: if the static variable array in the global, then i default the value 0,
**          but if it's in the function, then it's garbage value;
*/
typedef struct          s_env
{
    int                 port;
    int                 authorized_clients;
    int                 time_unit;
    int                 nb_team; // sding added it            
    int                 map[1024][1024][7]; //think of static array map[1024][1024][7] -> map[y][x][7];
    // t_map               map;  // sding delete it 
    int                 map_x;
    int                 map_y;
    t_players			*players;
    t_team				*teams;
	t_queue				*queue; // sding added it
	char				buffer[4096]; // sding added it
    int                 server_fd; // sding added it --> possible is the listener fd
}                       t_env;

t_env       g_env;
t_players   g_players[FD_SETSIZE]; // FD_SETSIZE = 1024



#endif
