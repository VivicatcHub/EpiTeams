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

void cmd_list_channels_for(server_t *server, team_t *team_ok,
    char **result, int *len)
{
    int max_buffer = MAX_BUFFER;

    for (channel_t *channel = server->data.channels.tqh_first;
        channel != NULL; channel = channel->entries.tqe_next) {
        if (uuid_compare(channel->team_uuid, team_ok->uuid) == 0) {
            cmd_list_channels2(len, result, channel, &max_buffer);
            *result = realloc(*result, max_buffer);
        }
    }
}

static int cmd_list_channels_check(server_t *server, client_t *client,
    char *args, team_t **team_ok)
{
    user_t *user = find_user_by_name(server, client->username);
    uuid_t uuid_args;
    team_t *team = find_team_by_name(server, args);
    team_t *team_;

    string_to_uuid(args, uuid_args);
    team_ = find_team_by_uuid(server, uuid_args);
    *team_ok = (!team) ? team_ : team;
    if (team_exist(server, client, args) == 0)
        return 0;
    if (require_user_subscribed_to_team(server, client, user, *team_ok) != 0)
        return 0;
    return 1;
}

void cmd_list_channels(server_t *server, client_t *client, char *args)
{
    int len = 0;
    char *result = malloc(sizeof(char) * (MAX_BUFFER + 1));
    team_t *team_ok = NULL;

    memset(result, 0, MAX_BUFFER + 1);
    if (!cmd_list_channels_check(server, client, args, &team_ok)) {
        free(result);
        return;
    }
    cmd_list_channels_for(server, team_ok, &result, &len);
    send_response(client, CODE_COMMAND_OK, result);
    free(result);
}
