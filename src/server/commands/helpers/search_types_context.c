/*
** EPITECH PROJECT, 2026
** MyTeams
** File description:
** thread/channel context verifications
*/

#include "my_teams.h"

static int thread_not_found(client_t *client)
{
    send_response(client, CODE_THREAD_NOT_FOUND, "Thread not found.");
    return -1;
}

static thread_t *find_thread_by_title_in_context(server_t *server,
    char const *title, team_t *team, channel_t *channel)
{
    thread_t *thread = NULL;

    TAILQ_FOREACH(thread, &server->data.threads, entries) {
        if (strcmp(thread->title, title) != 0)
            continue;
        if (team && uuid_compare(thread->team_uuid, team->uuid) != 0)
            continue;
        if (channel && uuid_compare(thread->channel_uuid, channel->uuid) != 0)
            continue;
        return thread;
    }
    return NULL;
}

static int verify_thread_context(client_t *client, thread_t *thread,
    team_t *team, channel_t *channel)
{
    if (team && uuid_compare(thread->team_uuid, team->uuid) != 0)
        return thread_not_found(client);
    if (channel && uuid_compare(thread->channel_uuid, channel->uuid) != 0)
        return thread_not_found(client);
    return 0;
}

static int parse_thread_uuid_in_context(server_t *server, client_t *client,
    uuid_t thread_uuid, thread_context_params_t *params)
{
    *params->out_thread = find_thread_by_uuid(server, thread_uuid);
    if (!*params->out_thread)
        return thread_not_found(client);
    return verify_thread_context(client, *params->out_thread,
        params->team, params->channel);
}

int parse_and_get_thread_in_context(server_t *server, client_t *client,
    thread_context_params_t *params)
{
    uuid_t thread_uuid;

    if (string_to_uuid(params->thread_str, thread_uuid) == 0)
        return parse_thread_uuid_in_context(server, client, thread_uuid,
            params);
    *params->out_thread = find_thread_by_title_in_context(server,
        params->thread_str, params->team, params->channel);
    if (!*params->out_thread)
        return thread_not_found(client);
    return 0;
}

int parse_and_verify_channel_in_team(server_t *server, client_t *client,
    channel_in_team_params_t *params)
{
    if (params->team_str && parse_and_verify_team(server, client,
        params->team_str, params->out_team) != 0)
        return -1;
    if (parse_and_get_channel(server, client, params->channel_str,
        params->out_channel) != 0)
        return -1;
    if (params->team_str && uuid_compare((*params->out_channel)->team_uuid,
        (*params->out_team)->uuid) != 0) {
        send_response(client, CODE_CHANNEL_NOT_FOUND, "Channel not found.");
        return -1;
    }
    if (params->team_str && !*params->out_team)
        *params->out_team = find_team_by_uuid(server,
            (*params->out_channel)->team_uuid);
    return 0;
}

int parse_and_verify_thread_in_channel(server_t *server, client_t *client,
    thread_in_channel_params_t *params)
{
    channel_in_team_params_t channel_params;
    thread_context_params_t thread_params;

    channel_params.team_str = params->team_str;
    channel_params.channel_str = params->channel_str;
    channel_params.out_team = params->out_team;
    channel_params.out_channel = params->out_channel;
    if ((params->team_str || params->channel_str) &&
        parse_and_verify_channel_in_team(server, client, &channel_params) != 0)
        return -1;
    thread_params.thread_str = params->thread_str;
    thread_params.team = *params->out_team;
    thread_params.channel = *params->out_channel;
    thread_params.out_thread = params->out_thread;
    return parse_and_get_thread_in_context(server, client, &thread_params);
}
