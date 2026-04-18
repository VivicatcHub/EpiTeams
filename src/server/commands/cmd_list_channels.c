/*
** EPITECH PROJECT, 2026
** G-NWP-400-LYN-4-1-mychannels-1
** File description:
** cmd_list_channel
*/

#include "my_teams.h"

void cmd_list_channels2(client_t *client, channel_t *channel)
{
    char uuid[UUID_STR_LEN];
    char buf[MAX_BUFFER];

    uuid_to_string(channel->uuid, uuid);
    snprintf(buf, MAX_BUFFER, "%s (%s) - %s", channel->name, uuid,
        channel->description);
    send_response(client, CODE_COMMAND_BEGIN, buf);
}

void cmd_list_channels_for(server_t *server, client_t *client,
    team_t *team_ok)
{
    for (channel_t *channel = server->data.channels.tqh_first;
        channel != NULL; channel = channel->entries.tqe_next) {
        if (uuid_compare(channel->team_uuid, team_ok->uuid) == 0)
            cmd_list_channels2(client, channel);
    }
}

void cmd_list_channels(server_t *server, client_t *client, char *args)
{
    team_t *team = NULL;

    if (is_user_logged_in(server, client) == 0)
        return;
    if (parse_and_verify_team(server, client, args, &team) != 0)
        return;
    cmd_list_channels_for(server, client, team);
    send_response(client, CODE_COMMAND_END, "No more channels.");
}
