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
    team_t *team_n = NULL;
    channel_t *channel = NULL;
    channel_t *channel_n = NULL;
    channel_t *channel_ok = NULL;
    uuid_t uuid;
    uuid_t uuid_team;

    string_to_uuid(args, uuid);
    channel = find_channel_by_uuid(server, uuid);
    channel_n = find_channel_by_name(server, args);
    if (!channel && !channel_n) {
        send_response(client, CODE_CHANNEL_NOT_FOUND, "Channel not found.");
        return 0;
    }
    channel_ok = (!channel) ? channel_n : channel;
    string_to_uuid(args, uuid_team);
    team = find_channel_by_uuid(server, uuid);
    team_n = find_team_by_uuid(server, channel_ok->team_uuid);
    if (!team_n && !team) {
        send_response(client, CODE_NOT_FOUND, "Team not found.");
        return 0;
    }
    return 1;
}


void cmd_info_channel(server_t *server, client_t *client, char *args)
{
    char result[MAX_BUFFER];
    char uuid_buf[UUID_STR_LEN];
    int64_t time_;
    channel_t *channel;
    channel_t *channel_n;
    channel_t *channel_ok;
    uuid_t uuid;

    if (is_team_exist(server, client, args) == 0)
        return;
    string_to_uuid(args, uuid);
    channel = find_channel_by_uuid(server, uuid);
    channel_n = find_channel_by_name(server, args);
    if (!channel && !channel_n) {
        send_response(client, CODE_NOT_FOUND, "Channel not found.");
        return;
    }
    channel_ok = (!channel) ? channel_n : channel;
    time_ = (int64_t)channel_ok->created_at;
    uuid_to_string(channel_ok->uuid, uuid_buf);
    snprintf(result, sizeof(result), "Channel Name : %s | Uuid : %s\n Created at %i | Description : %s",
        channel_ok->name, uuid_buf, time_, channel_ok->description);
    send_response(client, CODE_COMMAND_OK, result);
}
