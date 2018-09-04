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
#include <server.h>

int		cmd_inventory(t_players players, char *msg)
{
	printf(BLUE"Player [$d] -> [%s]\n"RESET, players.fd, "inventory");
	bzero(g_env.buffer, 4096);
	strcpy(g_env.buffer, "{food ");
	strcat(g_env.buffer, ft_itoa(players.inventory[0]));
	strcat(g_env.buffer, ", linemate ");
	strcat(g_env.buffer, ft_itoa(players.inventory[1]));
	strcat(g_env.buffer, ", deraumere ");
	strcat(g_env.buffer, ft_itoa(players.inventory[2]));
	strcat(g_env.buffer, ", sibur ");
	strcat(g_env.buffer, ft_itoa(players.inventory[3]));
	strcat(g_env.buffer, ", mendiane ");
	strcat(g_env.buffer, ft_itoa(players.inventory[4]));
	strcat(g_env.buffer, ", phiras ");
	strcat(g_env.buffer, ft_itoa(players.inventory[5]));
	strcat(g_env.buffer, ", thystame ");
	strcat(g_env.buffer, ft_itoa(players.inventory[6]));
	players.request_nb--;
	if (send_msg(players.fd, g_env.buffer, "Send [inventory]") == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
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
