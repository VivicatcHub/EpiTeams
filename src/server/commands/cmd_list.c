/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_list.c
*/

#include "my_teams.h"

void cmd_list(server_t *server, client_t *client, char *args)
{
    char result[MAX_BUFFER];
    int i = 0;

    for (team_t *team = server->data.teams.tqh_first; team != NULL; team = team->entries.tqe_next) {
        snprintf(result, sizeof(result), "Team %i : %s", team->name);
        send_response(client, CODE_COMMAND_OK, result);
        i++;
    }

    // cmd_placeholder(server, client, args);
}
