/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/05 13:38:27 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/10 23:09:03 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <errno.h>
# include "parse.h"

# define BUF_SIZE 32
# define MSG_SIZE 4096
# define MAP_SIZE 906
# define PLAYER_SIZE 35

# define MAX_FD 16
# define MAX_TEAM 4
// # define FAILURE_CHAR '$'
# define PAD_CHAR '#'

# define WELCOME "WELCOME 🙂"
# define TEAM_FULL "TEAM IS FULL\nBYE 😕"
# define NAME_NOT_FOUND "TEAM NAME NOT FOUND\nBYE 😕"

# define SELECT_VARS fd_set master, read_fds; int fdmax;

int		validate_req(char *cmd);
char	*recv_data(int fd, int ebytes);
int		send_data(int fd, char *data, int ebytes);

#endif
