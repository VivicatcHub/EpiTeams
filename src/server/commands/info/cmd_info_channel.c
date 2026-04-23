/*
** EPITECH PROJECT, 2026
** G-NWP-400-LYN-4-1-myteams-1
** File description:
** cmd_info_channel
*/

#include "my_teams.h"
#include <time.h>

int is_team_exist(server_t *server, client_t *client, char *args)
{
    team_t *team = NULL;
    channel_t *channel = NULL;
    channel_t *channel_n = NULL;
    channel_t *channel_ok = NULL;
    uuid_t uuid;

    string_to_uuid(args, uuid);
    channel = find_channel_by_uuid(server, uuid);
    channel_n = find_channel_by_name(server, args);
    if (!channel && !channel_n) {
        send_response(client, CODE_CHANNEL_NOT_FOUND, "Channel not found.");
        return 0;
    }
    channel_ok = (!channel) ? channel_n : channel;
    team = find_team_by_uuid(server, channel_ok->team_uuid);
    if (!team) {
        send_response(client, CODE_NOT_FOUND, "Team not found.");
        return 0;
    }
    return 1;
}

void cmd_info_channel_2(client_t *client, channel_t *channel_ok)
{
    char uuid_buf[UUID_STR_LEN];
    char result[MAX_BUFFER];
    char time_buf[64];
    time_t t = (time_t)channel_ok->created_at;
    struct tm tm_info;

    uuid_to_string(channel_ok->uuid, uuid_buf);
    localtime_r(&t, &tm_info);
    strftime(time_buf, sizeof(time_buf), "%d/%m/%Y %H:%M:%S", &tm_info);
    snprintf(result, sizeof(result),
        "Channel Name : %s | Uuid : %s\n Created at %s | Description : %s",
        channel_ok->name, uuid_buf, time_buf, channel_ok->description);
    send_response(client, CODE_COMMAND_OK, result);
}

static int get_channel_info(server_t *server, client_t *client, char *args,
    channel_t **channel_ok)
{
    uuid_t uuid;
    channel_t *channel = NULL;

    string_to_uuid(args, uuid);
    channel = find_channel_by_uuid(server, uuid);
    if (!channel)
        channel = find_channel_by_name(server, args);
    if (!channel) {
        send_response(client, CODE_NOT_FOUND, "Channel not found.");
        return 0;
    }
    *channel_ok = channel;
    return 1;
}

void cmd_info_channel(server_t *server, client_t *client, char *args)
{
    user_t *user = find_user_by_name(server, client->username);
    channel_t *channel_ok;
    team_t *team;

    if (is_team_exist(server, client, args) == 0)
        return;
    if (get_channel_info(server, client, args, &channel_ok) == 0)
        return;
    team = find_team_by_uuid(server, channel_ok->team_uuid);
    if (require_user_subscribed_to_team(server, client, user, team) != 0)
        return;
    cmd_info_channel_2(client, channel_ok);
}
