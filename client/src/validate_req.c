/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfeng <zfeng@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/07 13:37:05 by zfeng             #+#    #+#             */
/*   Updated: 2018/09/10 23:08:51 by zfeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

char	*g_cmd_table[] = {"advance", "right", "left", "see", "inventory", \
	"take", "put", "kick", "incantation", "fork", "connect_nbr", 0};

char	*g_objects[] = {"linemate", "deraumere", "sibur", "mendiane", \
	"phiras", "thystame", "food", 0};

// int		validate_req(char *req)
// {
// 	char	*cmd;
// 	char	*param;
// 	int		i;

// 	cmd = strtok(req, " ");
// 	param = strtok(NULL, " ");
// 	i = -1;
// 	if (0 == strcmp(cmd, "take") || 0 == strcmp(cmd, "put"))
// 	{
// 		while (g_objects[++i])
// 		{
// 			if (NULL != param && (0 == strcmp(g_objects[i], param)))
// 				return (EXIT_SUCCESS);
// 		}
// 		return (EXIT_FAILURE);
// 	}
// 	if (0 == strcmp(cmd, "broadcast"))
// 	{
// 		if (NULL != param)
// 			return (EXIT_SUCCESS);
// 		return (EXIT_FAILURE);
// 	}
// 	while (g_cmd_table[++i])
// 	{
// 		if (0 == strcmp(req, g_cmd_table[i]) && NULL == param)
// 			return (EXIT_SUCCESS);
// 	}
// 	return (EXIT_FAILURE);
// }


int		validate_req(char *req)
{
	char	buf[MSG_SIZE];
	char	*cmd;
	char	*param;
	int		i;

	if (0 == *req)
		return (EXIT_FAILURE);
	strcpy(buf, req);
	cmd = strtok(buf, " ");
	param = strtok(NULL, " ");
	i = -1;
	if (0 == strcmp(cmd, "take") || 0 == strcmp(cmd, "put"))
	{
		while (g_objects[++i])
		{
			if (NULL != param && (0 == strcmp(g_objects[i], param)))
				return (EXIT_SUCCESS);
		}
		return (EXIT_FAILURE);
	}
	if (0 == strcmp(cmd, "broadcast"))
	{
		if (NULL != param)
			return (EXIT_SUCCESS);
		return (EXIT_FAILURE);
	}
	while (g_cmd_table[++i])
	{
		if (0 == strcmp(req, g_cmd_table[i]) && NULL == param)
			return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}