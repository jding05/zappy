/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reqq.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/31 15:34:42 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/10 23:07:16 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "reqq.h"

void	enqueue(t_reqq **head, int fd, char *req)
{
	t_reqq	*new;
	t_reqq *tmp;
	char	*cmd;
	char	*param;

	cmd = strtok(req, " ");
	param = strtok(NULL, " ");
	printf("cmd = |%s|  param = |%s|\n", cmd, param);
	if (!*head)
	{
		if (!(*head = (t_reqq*)malloc(sizeof(t_reqq))))
			return ;
		(*head)->fd = fd;
		strcpy((*head)->cmd, cmd);
		if (param)
		{
			write(1, "here\n", 5);
			strcpy((*head)->param, param);
		}
		(*head)->next = NULL;
	}
	else
	{
		if (!(new = (t_reqq*)malloc(sizeof(t_reqq))))
			return ;
		new->fd = fd;
		strcpy(new->cmd, cmd);
		if (param)
			strcpy(new->param, param);
		new->next = NULL;
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	free(cmd);
	// if (param)
	// 	free(param);
}

void	dequeue(t_reqq **head)
{
	t_reqq	*tmp;

	if (!*head)
		return ;
	tmp = *head;
	*head = (*head)->next;
	free(tmp);
	tmp = NULL;
}


void	print_queue(t_reqq *head)
{
	t_reqq *tmp;

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
	t_reqq	*cmd;
	cmd = NULL;

	//cmd = (t_reqq*)malloc(sizeof(t_reqq));
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
