/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_inventory.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 18:25:44 by sding             #+#    #+#             */
/*   Updated: 2018/08/17 18:25:45 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** {phiras n, sibur n, ...}
*/
#include "../../include/server.h"

static void store_inventory_to_buffer(int fd)
{
	strcat(g_env.buffer, "food ");
	strcat(g_env.buffer, str = ft_itoa(g_players[fd].inventory[0]));
	free(str);
	strcat(g_env.buffer, ", linemate ");
	strcat(g_env.buffer, str = ft_itoa(g_players[fd].inventory[1]));
	free(str);
	strcat(g_env.buffer, ", deraumere ");
	strcat(g_env.buffer, str = ft_itoa(g_players[fd].inventory[2]));
	free(str);
	strcat(g_env.buffer, ", sibur ");
	strcat(g_env.buffer, str = ft_itoa(g_players[fd].inventory[3]));
	free(str);
	strcat(g_env.buffer, ", mendiane ");
	strcat(g_env.buffer, str = ft_itoa(g_players[fd].inventory[4]));
	free(str);
	strcat(g_env.buffer, ", phiras ");
	strcat(g_env.buffer, str = ft_itoa(g_players[fd].inventory[5]));
	free(str);
	strcat(g_env.buffer, ", thystame ");
	strcat(g_env.buffer, str = ft_itoa(g_players[fd].inventory[6]));
	free(str);
}

void		cmd_inventory(int fd, char *msg)
{
	char	*str;

	(void)msg;
	printf(CYAN"\n[Exec INVENTORY]\n"RESET);
	printf(BLUE"Player [%d] -> [%s]\n"RESET, fd, "inventory");
	bzero(g_env.buffer, MSG_SIZE);
	strcpy(g_env.buffer, RED"{");
	store_inventory_to_buffer(fd);
	strcat(g_env.buffer, "}"RESET);
	printf("players %d inventory: %s\n", fd, g_env.buffer);
	printf(CYAN"\n[INVENTORY SUCCESS]\n"RESET);
	g_players[fd].request_nb--;
	send_data(fd, g_env.buffer, MSG_SIZE);
}

/*
can consider using vdprintf(fd, msg, arg)
example:
vsend(players->fd, "{food %d, linemate %d, deraumere %d, sibur %d,
	mendiane %d, phiras %d, thstame %d", players->inventory[0],
	players->inventory[1], players->inventory[2], players->inventory[3],
	players->inventory[4], players->inventory[5], players->inventroy[6]);

void	vsend(int socket, char *msg, ...)
{
	va_list	ap;

	va_start(ap, msg);
	vdprintf(socket, msg, ap);
	va_end(ap);
}
** note: using dprintf() might be some issue,
**       cuz sometime dprintf is used for debugging
*/
