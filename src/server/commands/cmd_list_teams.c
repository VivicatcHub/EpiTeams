/*
** EPITECH PROJECT, 2026
** G-NWP-400-LYN-4-1-myteams-1
** File description:
** cmd_list_team
*/

#include "my_teams.h"

void cmd_list_teams(server_t *server, client_t *client, char *args)
{
    char result[MAX_BUFFER];
    int i = 0;

    for (team_t *team = server->data.teams.tqh_first; team != NULL; team = team->entries.tqe_next) {
        snprintf(result, sizeof(result), "Team : %s", team->name);
        send_response(client, CODE_COMMAND_OK, result);
        i++;
    }
}