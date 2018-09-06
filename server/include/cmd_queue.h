/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_queue.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/31 15:37:38 by zfeng             #+#    #+#             */
/*   Updated: 2018/08/31 16:52:45 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_QUEUE_H
# define CMD_QUEUE_H
# define MAX_CMD 1024
# define MAX_MSG 1024

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>

typedef struct		s_cmdq
{
	int				fd;
	char			cmd[MAX_CMD];
	char			msg[MAX_MSG];
	char			ret[1024];
	struct s_cmdq	*next;
}					t_cmdq;

void	enqueue(t_cmdq **head, int fd, char *cmd);
void	dequeue(t_cmdq **head);

void	print_queue(t_cmdq *head);
#endif