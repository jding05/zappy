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

int		send_data(int fd, char *data, int ebytes)
{
	int		nbytes;
	int		tbytes;
	char	*buf;
	int		i;

	buf = (char*)malloc(sizeof(char) * (ebytes + 1));
	memset(buf, 0, ebytes + 1);
	// printf("server inital data = |%s|\n", data);
	i = -1;
	while (data && data[++i])
		buf[i] = data[i];
	while (i < ebytes)
		buf[i++] = PAD_CHAR;
	buf[i] = '\0';
	tbytes = 0;
	while (1)
	{
		nbytes = send(fd, buf, ebytes - tbytes, 0);
		if (nbytes <= 0)
		{
			perror("send error\n");
			free(buf);
			return (0);
		}
		tbytes += nbytes;
		if (tbytes >= ebytes)
		{
			// printf("server send msg = |%s|\n", buf);
			free(buf);
			return (tbytes);
		}
	}
}

char	*recv_data(int fd, int ebytes)
{
	int		nbytes;
	int		tbytes;
	char	*buf;
	char	*data;
	int		i;

	buf = (char*)malloc(sizeof(char) * (ebytes + 1));
	data = (char*)malloc(sizeof(char) * (ebytes + 1));
	memset(buf , 0, ebytes + 1);
	memset(data, 0, ebytes + 1);
	tbytes = 0;
	while (1)
	{
		nbytes = recv(fd, buf, ebytes - tbytes, 0);
		if (nbytes < 0)
		{
			free(buf);
			free(data);
			perror("recv error\n");
			return (NULL);
		}
		if (nbytes == 0)
		{
			free(buf);
			free(data);
			return (NULL);
		}
		buf[nbytes] = '\0';
		tbytes += nbytes;
		i = -1;
		while (++i < ebytes && buf[i] != PAD_CHAR)
			;
		buf[i] = '\0';
		// printf("buf = |%s|\n", buf);
		strncat(data, buf, i);
		if (tbytes >= ebytes)
		{
			free(buf);
			return (data);
		}
	}
}
