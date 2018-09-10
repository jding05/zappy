/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_send_recv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/08 17:28:01 by sding             #+#    #+#             */
/*   Updated: 2018/09/08 17:28:20 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/server.h"
# define BUF_SIZE32 32

void	s_send_msg(int fd, char *msg)
{
	int		nbytes;
	int		tbytes;
	char	buf[BUF_SIZE32];
	int		i;

	printf("server inital msg = |%s|\n", msg);
	i = 0;
	while (msg[i])
	{
		buf[i] = msg[i];
		i++;
	}
	while (i < BUF_SIZE32 - 1)
	{
		buf[i] = '#';
		i++;
	}
	buf[i] = '\0';
	tbytes = 0;
	while (tbytes < BUF_SIZE32)
	{
		nbytes = send(fd, buf, BUF_SIZE32, 0);
		if (nbytes < 0)
		{
			perror("send error\n");
			return ;
		}
		tbytes += nbytes;
		if (tbytes == BUF_SIZE32)
		{
			printf("server send msg = |%s|\n", buf);
			memset(buf, 0, BUF_SIZE32);
			return ;
		}
	}
}

void	recv_print(int fd)
{
	int		nbytes;
	int		tbytes;
	char	buf[BUF_SIZE32];
	char	msg[BUF_SIZE32];
	int		i;

	tbytes = 0;
	while (tbytes < BUF_SIZE32)
	{
		nbytes = recv(fd, buf, BUF_SIZE32, 0);
		if (nbytes < 0)
		{
			perror("recv error\n");
			return ;
		}
		if (nbytes == 0)
		{
			// perror("sending stopped\n");
			s_init_new_player(fd);
			printf("Player on socket %d left\n", fd);
			return ;
		}
		buf[nbytes] = '\0';
		tbytes += nbytes;
		i = 0;
		while (i < BUF_SIZE32)
		{
			if (buf[i] == '#')
				buf[i] = '\0';
			i++;
		}
		strncat(msg, buf, i);
		printf("server recv tbytes = %d\n", tbytes);
		if (tbytes == BUF_SIZE32)
		{
			printf("server recv msg = |%s|\n", msg);
			break ;
		}
	}
	printf("server unpad msg = |%s|\n", msg);
}





// void	recv_print(int fd)
// {
// 	int		nbytes;
// 	int		tbytes;
// 	char	buf[BUF_SIZE];
// 	char	msg[BUF_SIZE];
// 	int		i;

// 	tbytes = 0;
// 	while (tbytes < BUF_SIZE)
// 	{
// 		write(1, "here\n", 1);
// 		nbytes = recv(fd, buf, BUF_SIZE, 0);
// 		if (nbytes < 0)
// 		{
// 			perror("recv error\n");
// 			return ;
// 		}
// 		if (nbytes == 0)
// 		{
// 			perror("sending stopped\n");
// 			break ;
// 		}
// 		buf[nbytes] = '\0';
// 		tbytes += nbytes;
// 		strncat(msg, buf, nbytes);
// 		printf("server recv tbytes = %d\n", tbytes);
// 		if (tbytes == BUF_SIZE)
// 		{
// 			printf("server recv msg = |%s|\n", msg);
// 			break ;
// 		}
// 	}
// 	i = 0;
// 	while (i < BUF_SIZE)
// 	{
// 		if (msg[i] == '#')
// 			msg[i] = '\0';
// 		i++;
// 	}
// 	printf("server unpad msg = |%s|\n", msg);

// }
