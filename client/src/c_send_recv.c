/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_recv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/07 18:25:18 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/07 18:39:55 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

void	send_msg(int fd, char *msg)
{
	int		nbytes;
	int		tbytes;
	char	buf[BUF_SIZE];
	int		i;

	printf("client inital msg = |%s|\n", msg);
	i = 0;
	while (msg[i])
	{
		buf[i] = msg[i];
		i++;
	}
	while (i < BUF_SIZE - 1)
	{
		buf[i] = '#';
		i++;
	}
	buf[i] = '\0';
	tbytes = 0;
	while (tbytes < BUF_SIZE)
	{
		nbytes = send(fd, buf, BUF_SIZE, 0);
		if (nbytes < 0)
		{
			perror("client send error\n");
			return ;
		}
		tbytes += nbytes;
		if (tbytes == BUF_SIZE)
		{
			printf("client send msg = |%s|\n", buf);
			memset(buf, 0, BUF_SIZE);
			return ;
		}
	}
}


void	recv_print(int fd)
{
	int		nbytes;
	int		tbytes;
	char	buf[BUF_SIZE];
	char	msg[BUF_SIZE];
	int		i;

	tbytes = 0;
	while (tbytes < BUF_SIZE)
	{
		nbytes = recv(fd, buf, BUF_SIZE, 0);
		if (nbytes < 0)
		{
			perror("recv error\n");
			return ;
		}
		if (nbytes == 0)
		{
			perror("sending stopped\n");
			return ;
		}
		buf[nbytes] = '\0';
		printf("client buf = |%s|\n", buf);
		tbytes += nbytes;
		i = 0;
		while (i < BUF_SIZE)
		{
			if (buf[i] == '#')
				buf[i] = '\0';
			i++;
		}
		strncpy(msg, buf, i);
		memset(buf, 0, BUF_SIZE);
		printf("client recv tbytes = %d\n", tbytes);
		if (tbytes == BUF_SIZE)
		{
			printf("client recv msg = |%s|\n", msg);
			break ;
	
		}
	}

	printf("client unpad msg = |%s|\n", msg);
}



