/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_recv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/07 18:25:18 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/16 09:58:07 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/server.h"

int		send_data_norm(int nbytes, char **buf)
{
	if (nbytes <= 0)
	{
		free(*buf);
		ERROR("send error");
	}
	return (EXIT_SUCCESS);
}

int		send_data(int fd, char *data, int ebytes)
{
	int		nbytes;
	int		tbytes;
	char	*buf;
	int		i;

	buf = ft_strnew(ebytes + 1);
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
		if (EXIT_FAILURE == send_data_norm(nbytes, &buf))
			return (EXIT_FAILURE);
		tbytes += nbytes;
		if (tbytes >= ebytes)
		{
			free(buf);
			return (EXIT_SUCCESS);
		}
	}
}

int		recv_data_norm(int fd, int nbytes, char **buf, char **data)
{
	if (nbytes < 0)
	{
		free(*buf);
		free(*data);
		perror("recv error\n");
		return (0);
	}
	if (nbytes == 0)
	{
		free(*buf);
		free(*data);
		printf("socket %d left\n", fd);
		g_players[fd].left = 1;
		return (0);
	}
	return (1);
}

void	recv_data_skip_pad(int ebytes, char **buf, char **data)
{
	int		i;

	i = -1;
	while (++i < ebytes && (*buf)[i] != PAD_CHAR)
		;
	(*buf)[i] = '\0';
	strncat(*data, *buf, i);
}

char	*recv_data(int fd, int ebytes)
{
	int		nbytes;
	int		tbytes;
	char	*buf;
	char	*data;

	buf = ft_strnew(ebytes + 1);
	data = ft_strnew(ebytes + 1);
	tbytes = 0;
	while (1)
	{
		nbytes = recv(fd, buf, ebytes - tbytes, 0);
		if (0 == recv_data_norm(fd, nbytes, &buf, &data))
			return (NULL);
		buf[nbytes] = '\0';
		tbytes += nbytes;
		recv_data_skip_pad(ebytes, &buf, &data);
		if (tbytes >= ebytes)
		{
			free(buf);
			return (data);
		}
	}
}
