/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/10 23:18:17 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/10 23:18:20 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

# include "reqq.h"
# include "parse.h"
# include "cmds.h"

# define MSG_SIZE 32
# define MAP_SIZE 906
# define PLAYER_SIZE 35
# define REQ_SIZE 64

# define MAX_FD 16
# define MAX_TEAM 4
# define MAX_TEAM_NAME 32
# define FAILURE_CHAR '$'
# define PAD_CHAR '#'

# define WELCOME "WELCOME 🙂"
# define TEAM_FULL "TEAM IS FULL\nBYE 😕"
# define NAME_NOT_FOUND "TEAM NAME NOT FOUND\nBYE 😕"

# define SOCKET_VARS struct addrinfo hints, *ai; struct protoent *proto;
# define SELECT_VARS fd_set master, read_fds; int fdmax;

typedef struct		s_player
{
	int				fd;
	int				player_id;
	int				team_id;
	int				inventory[7];
	int				pos[2];
	int				orientation;
	int				nb_req;
	int				level;
	int				dead;
}					t_player;

typedef struct		s_team
{
	int				team_id;
	char			team_name[MAX_TEAM_NAME];
	int				nb_client;
	int				connect_nbr;
	int				reach_max_level;

}					t_team;


extern t_player		g_players[MAX_FD];
extern t_team		g_teams[MAX_TEAM];
extern t_reqq		*g_reqq;


int		perror_rv(char *errmsg);
void	init_player(int fd);
void	reset_player(int fd);
int		add_to_team(char *team_name, int fd);
void	exec_cmd(t_reqq **head);
int		send_data(int fd, char *msg, int ebytes);
char	*recv_data(int fd, int ebytes);

#endif
