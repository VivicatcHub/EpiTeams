/*
** EPITECH PROJECT, 2026
** G-NWP-400-LYN-4-1-myteams-1
** File description:
** cmd_info_team
*/

#include "my_teams.h"

void cmd_info_team(server_t *server, client_t *client, char *args)
{
    int exist = 0;
    char result[MAX_BUFFER];
    char uuid_buf[UUID_STR_LEN];

    for (team_t *team = server->data.teams.tqh_first; team != NULL;
        team = team->entries.tqe_next) {
        if (strcmp(args, team->name) == 0) {
            uuid_to_string(team->uuid, uuid_buf);
            snprintf(result, sizeof(result), "Team name : %s | Uuid : %s\n",
                team->name, uuid_buf);
            send_response(client, CODE_COMMAND_OK, result);
            exist = 1;
        }
    }
    if (exist == 0) {
        snprintf(result, sizeof(result), "Team name : %s doesn't exist",
            args);
        send_response(client, CODE_NOT_FOUND, result);
    }
}
