/*
** EPITECH PROJECT, 2026
** G-NWP-400-LYN-4-1-mychannels-1
** File description:
** cmd_list_channel
*/

#include "my_teams.h"

int team_exist(server_t *server, client_t *client, char *args)
{
    uuid_t uuid;
    team_t *team;
    team_t *team_;

    string_to_uuid(args, uuid);
    team = find_team_by_uuid(server, uuid);
    team_ = find_team_by_name(server, args);
    if (!team && !team_) {
        send_response(client, CODE_NOT_FOUND, "Team not found.");
        return 0;
    }
    return 1;
}

void cmd_list_channels2(int *len, char **result,
    channel_t *channel, int *max_buffer)
{
    char uuid[MAX_BUFFER];

    uuid_to_string(channel->uuid, uuid);
    *len = strlen(channel->name);
    strcat(*result, channel->name);
    *len += 3;
    strcat(*result, " | ");
    *len += strlen(uuid);
    strcat(*result, uuid);
    *len += 3;
    strcat(*result, " | ");
    *len += strlen(channel->description);
    strcat(*result, channel->description);
    *len += 1;
    strcat(*result, "\n");
    *max_buffer += *len;
}

void cmd_list_channels(server_t *server, client_t *client, char *args)
{
    int max_buffer = MAX_BUFFER;
    char *result = malloc(sizeof(char) * (max_buffer + 1));
    int len = 0;

    memset(result, 0, max_buffer + 1);
    if (team_exist(server, client, args) == 0) {
        free(result);
        return;
    }
    for (channel_t *channel = server->data.channels.tqh_first;
        channel != NULL; channel = channel->entries.tqe_next) {
        cmd_list_channels2(&len, &result, channel, &max_buffer);
        result = realloc(result, max_buffer);
    }
    send_response(client, CODE_COMMAND_OK, result);
    free(result);
}
