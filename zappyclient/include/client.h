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

# define BUF_SIZE 32
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
# include "parse.h"

int		validate_cmd(char *cmd);
char	*recv_data(int fd, int ebytes);
int		send_data(int fd, char *data, int ebytes);

#endif
