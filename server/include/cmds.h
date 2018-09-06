#ifndef CMDS_H
# define CMDS_H
# include "server.h"

typedef struct	s_cmd
{
	char	*cmd;
	int		delay_time;
	void	(*func)();
}				t_cmd;

void	cmd_advance(void);
void	cmd_broadcast(void);
void	cmd_connect_nbr(void);
void	cmd_fork(void);
void	cmd_hatch(void);
void	cmd_incantation(void);
void	cmd_incantation_check(void);
void	cmd_inventory(void);
void	cmd_kick(void);
void	cmd_left(void);
void	cmd_put(void);
void	cmd_right(void);
void	cmd_see(void);
void	cmd_take(void);

extern t_cmd	g_cmds[12];

#endif
