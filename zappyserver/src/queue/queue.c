/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/31 15:34:42 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/04 16:24:25 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_queue.h"

#include <unistd.h>
#include <stdio.h>
// # include "../../inc/server.h"


void	enqueue(t_cmdq **head, int fd, char *cmd)
{
	t_event	*new;
	t_event	*tmp;

	if (!*head)
	{
		//write(1, "haha\n", 5);
		if (!(*head = (t_event *)malloc(sizeof(t_event))))
			return ;
		(*head)->fd = fd;
		strcpy((*head)->cmd, cmd);
		memset((*head)->ret, 0, 1024);
		(*head)->next = NULL;
	}
	else
	{
		//write(1, "hehe\n", 5);
		if (!(new = (t_cmdq*)malloc(sizeof(t_cmdq))))
			return ;
		new->fd = fd;
		strcpy(new->cmd, cmd);
		memset(new->ret, 0, 1024);
		new->next = NULL;
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	dequeue(t_cmdq **head)
{
	t_cmdq	*tmp;

	if (!*head)
		return ;
	tmp = *head;
	*head = (*head)->next;
	free(tmp);
	tmp = NULL;
}
