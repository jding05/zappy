#ifndef SERVER_H
# define SERVER_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <sys/types.h>
# include <ctype.h>
# include <sys/time.h>
# include <math.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <errno.h>
# include <sys/select.h>

# include "cmd_queue.h"
# include "parse.h"
# include "cmds.h"


# define BUF_SIZE 256
# define MAX_FD 16
# define MAX_TEAM 4
# define MAX_TEAM_NAME 32
//# define SUCCESS_CHAR $
# define FAILURE_CHAR #

# define SOCKET_VARS struct addrinfo hints, *ai; struct protoent *proto;
# define SELECT_VARS fd_set master, read_fds; int fdmax;

# define WELCOME_MSG "WELCOME 🙂\n"
//# define TEAM_FULL_MSG "TEAM IS FULL\nBYE 😕\n"

typedef struct		s_player
{
	int				fd;
	int				team_id;
	int				inventory[7];
	int				pos[2];
	int				nb_req;
	int				level;
	int				dead;
}					t_player;

typedef struct		s_team
{
	int				team_id;
	char			team_name[MAX_TEAM_NAME];
	int				nb_client;
	int				reach_max_level;

}					t_team;


extern t_player		g_players[MAX_FD];
extern t_team		g_teams[MAX_TEAM];
extern t_cmdq		*g_cmds;

#endif
