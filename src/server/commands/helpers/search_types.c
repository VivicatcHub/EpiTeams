/*
** EPITECH PROJECT, 2026
** MyTeams
** File description:
** team_verif.c
*/

#include "my_teams.h"

int parse_and_verify_team(server_t *server, client_t *client, char *team_str,
    team_t **out_team)
{
    uuid_t team_uuid;

    if (string_to_uuid(team_str, team_uuid) != 0)
        *out_team = find_team_by_name(server, team_str);
    else
        *out_team = find_team_by_uuid(server, team_uuid);
    if (!*out_team) {
        send_response(client, CODE_NOT_FOUND, "Team not found.");
        return -1;
    }
    return 0;
}

int parse_and_get_channel(server_t *server, client_t *client,
    char *channel_str, channel_t **out_channel)
{
    uuid_t chan_uuid;

    if (string_to_uuid(channel_str, chan_uuid) != 0)
        *out_channel = find_channel_by_name(server, channel_str);
    else
        *out_channel = find_channel_by_uuid(server, chan_uuid);
    if (!*out_channel) {
        send_response(client, CODE_NOT_FOUND, "Channel not found.");
        return -1;
    }
    return 0;
}

int parse_and_get_thread(server_t *server, client_t *client, char *thread_str,
    thread_t **out_thread)
{
    uuid_t thread_uuid;

    if (string_to_uuid(thread_str, thread_uuid) != 0)
        *out_thread = find_thread_by_title(server, thread_str);
    else
        *out_thread = find_thread_by_uuid(server, thread_uuid);
    if (!*out_thread) {
        send_response(client, CODE_NOT_FOUND, "Thread not found.");
        return -1;
    }
    return 0;
}
