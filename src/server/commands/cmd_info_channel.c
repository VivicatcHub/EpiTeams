/*
** EPITECH PROJECT, 2026
** G-NWP-400-LYN-4-1-myteams-1
** File description:
** cmd_info_channel
*/

#include "my_teams.h"

int is_team_exist(server_t *server, client_t *client, char *args)
{
    uuid_t team_uuid;
    team_t *team = NULL;
    channel_t *channel = NULL;

    channel = find_channel_by_name(server, args);
    if (channel == NULL) {
        send_response(client, CODE_CHANNEL_NOT_FOUND, "Channel not found.");
        return 0;
    }
    team = find_team_by_uuid(server, channel->team_uuid);
    if (team == NULL) {
        send_response(client, CODE_NOT_FOUND, "Team not found.");
        return 0;
    }
    return 1;
}

void cmd_info_channel(server_t *server, client_t *client, char *args)
{
    int exist = 0;
    char result[MAX_BUFFER];
    char uuid_buf[UUID_STR_LEN];
    int64_t time_;
    uuid_t channel_uuid;

    if (is_team_exist(server, client, args) == 0)
        return;
    for (channel_t *channel = server->data.channels.tqh_first; channel != NULL;
        channel = channel->entries.tqe_next) {
        time_ = (int64_t)channel->created_at;

        if (strcmp(args, channel->name) == 0) {
            uuid_to_string(channel->uuid, uuid_buf);
            snprintf(result, sizeof(result), "Channel name : %s | Uuid : %s\nCreated at %i | Description : %s\n",
                channel->name, uuid_buf, time_, channel->description);
            send_response(client, CODE_COMMAND_OK, result);
            exist = 1;
        }
    }
    if (exist == 0) {
        snprintf(result, sizeof(result), "Channel name : %s doesn't exist", args);
        send_response(client, CODE_CHANNEL_NOT_FOUND, result);
    }
}