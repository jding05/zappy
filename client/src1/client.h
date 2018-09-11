/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/05 13:38:27 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/08 17:02:02 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# define BUF_SIZE 32
# define MAX_FD 16
# define MAX_TEAM 4
# define MAX_TEAM_NAME 32
# define FAILURE_CHAR '$'

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
# include "cparse.h"

int		validate_cmd(char *cmd);
void	recv_print(int fd);
void	send_msg(int fd, char *msg);

#endif
