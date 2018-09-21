#ifndef DATA_STRUCTURES_H
# define DATA_STRUCTURES_H

# include "server.h"
# include "queue.h"

typedef struct      s_egg
{
	int				team_id;
	int             egg_id;
	int             hatched;
	int             ffd;
	int             y;
	int             x;
}                   t_egg;

typedef struct      s_team
{
	int             team_id;
	char            team_name[MAX_TEAM_NAME];
	int             cplayers;
	int             max_players;
	int             reach_max_level;
	int				nb_client;
	t_egg           egg[6];
	int             egg_hatched;
	int             egg_laid;
}                   t_team;

typedef struct      s_player
{
	int				fd;
	int             player_id;
	int				team_id;
	int				inventory[7];
	int				y;
	int				x;
	int				request_nb;
	int				level;
	int				dead;
	int				left;
	int             status;
	int				block;
	int				direction;
	int				alive;
	struct timeval	live;
	struct timeval	block_time;
}                   t_player;

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
	char				port_name[6];
	int                 authorized_clients;
	int                 time_unit;
	int                 nb_team;
	int                 map[map_xy][map_xy][7];
	int                 map_x;
	int                 map_y;
	t_event             *queue_head;
	char                buffer[MSG_SIZE];
	int                 server_fd;
	long int			ms_pre_tick;
	int					res[7];
	int                 gfx_fd;
}                       t_env;


typedef struct			s_cmd
{
	char				*cmd;
	int					delay_time;
	void				(*func)(int fd, char *msg);
}						t_cmd;

t_env       			g_env;
t_team      			g_teams[MAX_TEAM];
t_player                g_players[MAX_FD];

/*
** declaring the (global) array variable
** Fact: the C compiler will ignore any text between the brackets [...]
**		in any array declaration
*/

extern const t_cmd 		g_cmd[];
extern const int		g_max_res[];
extern const char		*g_res_name[];
extern const char		*g_options[];

int     g_player_id;

/*
** ^ suppose the C compiler processed the following array declaration
** the C compiler will handle accessing the array element g_cmd[i] as follows:
** -> it generate a dummy base address x,
** -> the C compiler will use the <type> information, (int is 4 bytes) to
** generate the address of x + i * 4 for the array element g_max_res[i]
** this memory will be in the RAM memory
*/

#endif
