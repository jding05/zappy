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

int		validate_cmd_with_obj(char *str, char *cmd)
{
	int		i;
	int		n;

	i = 0;
	n = strlen(cmd);
	if (strnstr(str, cmd, n))
	{
		//printf("str = %s | cmd = %s | %s\n", str, cmd, strnstr(str, cmd, n));
		if (strcmp(strnstr(str, cmd, n), str) == 0)
		{
			while (g_objects[i])
			{
				//printf("%s\n", &(str[n + 1]));
				if (str[n] == ' ' && str[n + 1] && strcmp(&str[n + 1], g_objects[i]) == 0)
				{
					//printf("%s\n", g_objects[i]);
					return (EXIT_SUCCESS);
				}
				i++;
			}
		}
	}
	return (EXIT_FAILURE);
}

int		validate_broadcast(char *str)
{
	if (strnstr(str, "broadcast", 9))
	{
		if (strcmp(strnstr(str, "broadcast", 9), str) == 0)
			if (str[9] == ' ' && str[10])
				return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

int		validate_cmd(char *str)
{
	int		i;

	i = 0;
	while (g_cmd_table[i])
	{
		if (strcmp(g_cmd_table[i], str) == 0)
			return (EXIT_SUCCESS);
		if (validate_cmd_with_obj(str, g_cmd_table[i]) == EXIT_SUCCESS)
			return (EXIT_SUCCESS);
		if (validate_broadcast(str) == EXIT_SUCCESS)
			return (EXIT_SUCCESS);
		i++;
	}
	return (EXIT_FAILURE);
}
