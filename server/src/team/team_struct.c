/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   team_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sding <sding@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/29 19:19:08 by sding             #+#    #+#             */
/*   Updated: 2018/08/29 19:19:09 by sding            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <server.h>

void    print_team(t_team *head)
{
    t_team  *list;

    list = head;
    printf("\n[team details]\n\n");
    while (list)
    {
        printf("team_number: %d\n", list->team_number);
        printf("team_name: %s\n", list->team_name);
        printf("max_players: %d\n", list->max_players);
        printf("connected_players: %d\n", list->connected_players);
        printf("--------------------------------------\n");
        list = list->next;
    }
}

void    update_max_player_per_team(t_team *head)
{
    t_team  *list;
    int     max_players;

    max_players = g_env.authorized_clients / g_env.nb_team;
    list = head;
    while (list)
    {
        list->max_players = max_players;
        list = list->next;
    }
}

t_team  *team_node(char *team_name, int team_nb)
{
    t_team  *team;

    if (!(team = (t_team *)malloc(sizeof(t_team))))
        return (NULL);
    bzero(team, sizeof(t_team));
    team->team_name = strdup(team_name);
    team->team_number = team_nb;

    return (team);
}

t_team  *team_init(char **argv, int i, int nb_team)
{
    t_team  *head;
    t_team  *list;
    int     team_nbr;

    team_nbr = 0;
    head = team_node(argv[i++], team_nbr++);
    list = head;
    while (team_nbr < nb_team)
    {
        list->next = team_node(argv[i++], team_nbr++);
        list = list->next;
    }
    list->next = NULL;
    return (head);
}
