#include "cmds.h"

void	cmd_advance(void)
{
	printf("\n******advance executed******\n");
}

void	cmd_broadcast(void)
{
	printf("\n******boradcast executed******\n");
}

void	cmd_connect_nbr(void)
{
	printf("\n******connect_nbr executed******\n");
}

void	cmd_fork(void)
{
	printf("\n******fork executed******\n");
}

void	cmd_hatch(void)
{
	printf("\n******hatch executed******\n");
}

void	cmd_incantation(void)
{
	printf("\n******incantation executed******\n");
}

void	cmd_inventory(void)
{
	printf("\n******inventory executed******\n");
}

void	cmd_kick(void)
{
	printf("\n******kick executed******\n");
}

void	cmd_left(void)
{
	printf("\n******left executed******\n");
}

void	cmd_put(void)
{
	printf("\n******put executed******\n");
}

void	cmd_right(void)
{
	printf("\n******right executed******\n");
}

void	cmd_take(void)
{
	printf("\n******take executed******\n");
}

t_cmd	g_cmds[12] = 
{
	{"advance", 7, cmd_advance},
	{"broadcast", 7, cmd_broadcast},
	{"connect_nbr", 0, cmd_connect_nbr},
	{"fork", 42, cmd_fork},
	{"hatch", 600, cmd_hatch},
	{"incantation", 300, cmd_incantation},
	{"inventory", 1, cmd_inventory},
	{"kick", 7, cmd_kick},
	{"left", 7, cmd_left},
	{"put", 7, cmd_put},
	{"right", 7, cmd_right},
	{"take", 7, cmd_take},
};
