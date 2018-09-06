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

void	enqueue(t_cmdq **head, int fd, char *cmd)
{
	t_cmdq	*new;
	t_cmdq *tmp;

	if (!*head)
	{
		if (!(*head = (t_cmdq*)malloc(sizeof(t_cmdq))))
			return ;
		(*head)->fd = fd;
		strcpy((*head)->cmd, cmd);
		memset((*head)->ret, 0, 1024);
		(*head)->next = NULL;
	}
	else
	{
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
	write(1, "added to cmdq\n", 14);
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


void	print_queue(t_cmdq *head)
{
	t_cmdq *tmp;

	tmp = head;
	while (tmp)
	{
		printf("%s -> ", tmp->cmd);
		tmp = tmp->next;
	}
}

/*
int		main(void)
{
	t_cmdq	*cmd;
	cmd = NULL;

	//cmd = (t_cmdq*)malloc(sizeof(t_cmdq));
	enqueue(&cmd, 4, "I am the first");
	print_queue(cmd);
	enqueue(&cmd, 5, "hello world");
	print_queue(cmd);
	// printf("fd = %d\n", cmd->next->fd);
	// printf("cmd = |%s|\n", cmd->next->cmd);
	// write(1, "here\n", 5);
	return (0);
}
*/