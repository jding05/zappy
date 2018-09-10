/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_send.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/08 17:31:40 by sding             #+#    #+#             */
/*   Updated: 2018/09/08 17:31:41 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// # include "../../inc/server.h"

/*
void	s_send_msg(int fd, char *msg)
{
	int		i;
	int		j;
	int		l;
	char	buf[BUF_SIZE];

	i = BUF_SIZE;
	l = strlen(msg);
	while (l > 0)
	{
		buf[i] = l % 10 + '0';
		l = l / 10;
		i--;
	}
	j = 0;
	while (++i <= BUF_SIZE)
	{
		buf[j] = buf[i];
		buf[i] = 0;
		j++;
	}
	buf[j] = '#';
	strcat(buf, msg);
	//printf("%s", buf);
	send(fd, buf, BUF_SIZE, 0);
}
*/

// void	s_send_msg(int fd, char *msg)
// {
// 	int		nbytes;
// 	int		tbytes;
// 	char	buf[BUF_SIZE];
// 	int		i;

// 	i = 0;
// 	while (i < BUF_SIZE)
// 	{
// 		if (msg[i])
// 			buf[i] = msg[i];
// 		else
// 			buf[i] = '#';
// 		i++;
// 	}
// 	printf("buf = |%s|\n", buf);
// 	tbytes = 0;
// 	while (tbytes < BUF_SIZE)
// 	{
// 		nbytes = send(fd, buf, BUF_SIZE, 0);
// 		if (nbytes < 0)
// 		{
// 			perror("send error\n");
// 			return ;
// 		}
// 		tbytes += nbytes;
// 	}
// }


// below is the last version *********************

// void	s_send_msg(int fd, char *msg)
// {
// 	int		nbytes;
// 	int		tbytes;
// 	char	buf[BUF_SIZE];
// 	int		i;
//
// 	memset(buf, 9, BUF_SIZE);
// 	printf("smsg = |%s|\n", msg);
// 	i = 0;
// 	while (msg[i])
// 	{
// 		buf[i] = msg[i];
// 		i++;
// 	}
// 	while (i < BUF_SIZE - 1)
// 	{
// 		buf[i] = '#';
// 		i++;
// 	}
// 	buf[i] = '\0';
// 	printf("buf = |%s|\n", buf);
// 	tbytes = 0;
// 	while (tbytes < BUF_SIZE)
// 	{
// 		nbytes = send(fd, buf, BUF_SIZE, 0);
// 		if (nbytes <= 0)
// 		{
// 			perror("send error\n");
// 			return ;
// 		}
// 		tbytes += nbytes;
// 	}
// }
