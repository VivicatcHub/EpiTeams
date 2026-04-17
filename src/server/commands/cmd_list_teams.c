/*
** EPITECH PROJECT, 2026
** G-NWP-400-LYN-4-1-myteams-1
** File description:
** cmd_list_team
*/

#include "my_teams.h"

void cmd_list_teams(server_t *server, client_t *client, char *args)
{
    int max_buffer = MAX_BUFFER;
    char *result = malloc(sizeof(char) * (max_buffer + 1));
    int len = 0;
    char *uuid = malloc(sizeof(char) * (max_buffer + 1));

    memset(result, 0, max_buffer + 1);
    for (team_t *team = server->data.teams.tqh_first; team != NULL; team = team->entries.tqe_next) {
        uuid_to_string(team->uuid, uuid);
        len = strlen(team->name);
        strcat(result, team->name);
        len += 3;
        strcat(result, " | ");
        len += strlen(uuid);
        strcat(result, uuid);
        len += 3;
        strcat(result, " | ");
        len += strlen(team->description);
        strcat(result, team->description);
        len += 1;
        strcat(result, "\n");
        max_buffer += len;
        result = realloc(result, max_buffer);
        uuid = realloc(uuid, max_buffer);
    }
    send_response(client, CODE_COMMAND_OK, result);
    free(result);
    free(uuid);
}