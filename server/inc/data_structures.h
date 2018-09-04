#ifndef DATA_STRUCTURES_H
# define DATA_STRUCTURES_H


# include "server.h"
# include "queue.h"
// # include "cmd.h"
# include <sys/time.h>

# define MAX_TEAM_NAME 32

typedef struct      s_egg
{
    int				team_id;
    int             egg_id;
    int             hatched;
    int             direction;
    int             father_fd;
    int             y;
    int             x;
}                   t_egg;

typedef struct      s_team
{
    int             team_id;        // zfeng
    char            team_name[MAX_TEAM_NAME];
    int             connected_players;  // zfeng

    int             max_players;        // sding add it
    int             reach_max_level;    // sding added it
	int				nb_client; // -> need to check this **********

    t_egg           egg[10]; // need to added it
    int             egg_hatched;     // sding added it
    int             egg_laid;        // sding added it
}                   t_team;

typedef struct      s_players
{
	int				fd;
    int             team_id;
	int				inventory[7];
	int				y;
	int				x;
	int				request_nb;
    int             level;
    int             dead;

    int             block;
	int				direction;	// sding added it --> 0: N, 1: E, 2: S, 3: W
    struct timeval	live;
}                   t_players;

/*
** might not use this struct, but leave it here for resource reference
** int                 food         0
** int                 linemate;    1
** int                 deraumere;   2
** int                 sibur;       3
** int                 mendiane;    4
** int                 phiras;      5
** int                 thystame;    6
*/

/*
** note: if the static variable array in the global, then i default the value 0,
**          but if it's in the function, then it's garbage value;
*/

typedef struct          s_env
{
    int                 port;
	char				port_name[6];
    int                 authorized_clients;
    int                 time_unit;
    int                 nb_team;
    int                 map[1024][1024][7];
    int                 map_x;
    int                 map_y;
	t_st_queue			*st_queue;
    t_lt_queue          *lt_queue;
	char				buffer[4096];
    int                 server_fd;
	struct timeval		time_speed;
}                       t_env;


typedef struct	s_cmd
{
	char		*cmd;
	int			delay_time;
	int		(*func)(t_players players, char *msg);
}				t_cmd;


# define MAX_TEAM 4
# define MAX_FD 16

t_env       g_env;
t_team      g_teams[MAX_TEAM];
t_players   g_players[MAX_FD];
t_cmd	    g_cmd[14];


#endif
