/*
** EPITECH PROJECT, 2026
** MyTeams
** File description:
** team_verif.c
*/

#include "my_teams.h"

int is_user_logged_in(server_t *server, client_t *client)
{
    if (!client || !client->username ||
        !find_user_by_name(server, client->username)) {
        send_response(client, CODE_NOT_LOGGED_IN, "Not logged in.");
        return 0;
    }
    return 1;
}

int is_user_subscribed_to_team(user_t *user, uuid_t team_uuid)
{
    team_subscription_t *sub = NULL;

    if (!user)
        return 0;
    TAILQ_FOREACH(sub, &user->subscribed_teams, entries) {
        if (uuid_compare(sub->team_uuid, team_uuid) == 0)
            return 1;
    }
    return 0;
}

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
        send_response(client, CODE_CHANNEL_NOT_FOUND, "Channel not found.");
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
        send_response(client, CODE_THREAD_NOT_FOUND, "Thread not found.");
        return -1;
    }
    return 0;
}
