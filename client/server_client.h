#ifndef SERVER_CLIENT_H
# define SERVER_CLIENT_H

# define BUF_SIZE 256
# define MAX_FD 16
# define MAX_TEAM 4
# define MAX_TEAM_NAME 32

# define SOCKET_VARS struct addrinfo hints, *ai; struct protoent *proto;
# define SELECT_VARS fd_set master, read_fds; int fdmax;

# define WELCOME_MSG "WELCOME 🙂\n"
# define TEAM_FULL_MSG "TEAM IS FULL\nBYE 😕\n"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <errno.h>

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
}					t_team;


extern t_player		g_players[MAX_FD];
extern t_team		g_teams[MAX_TEAM];


///////////// debug ////////////////
void				print_players(void);


#endif
