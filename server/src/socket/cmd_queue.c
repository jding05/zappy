/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdq.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/31 15:34:42 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/01 12:36:53 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_queue.h"

#include <unistd.h>
#include <stdio.h>



void	enqueue(t_cmdq **head, int fd, char *cmd)
{
	t_cmdq	*new;
	t_cmdq *tmp;

	if (!*head)
	{
		//write(1, "haha\n", 5);
		if (!(*head = (t_cmdq*)malloc(sizeof(t_cmdq))))
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

/*
int		main(void)
{
	t_cmdq	*cmd;
	cmd = NULL;

	//cmd = (t_cmdq*)malloc(sizeof(t_cmdq));
	enqueue(&cmd, 4, "I am the first");
	enqueue(&cmd, 5, "hello world");
	printf("fd = %d\n", cmd->next->fd);
	printf("cmd = |%s|\n", cmd->next->cmd);
	write(1, "here\n", 5);
	return (0);
}
*/
