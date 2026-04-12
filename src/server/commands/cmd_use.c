/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_use.c
*/

#include "my_teams.h"

static char *next_token(char **str)
{
    char *s = NULL;
    char *space = NULL;

    if (!str)
        return NULL;
    s = *str;
    if (!s || s[0] == '\0')
        return NULL;
    space = strchr(s, ' ');
    if (space) {
        *space = '\0';
        *str = space + 1;
        return s;
    }
    *str = NULL;
    return s;
}

static int parse_use_args(char *args, char **out_team, char **out_channel,
    char **out_thread)
{
    char *p = NULL;

    if (!args || args[0] == '\0')
        return -1;
    p = args;
    *out_team = next_token(&p);
    *out_channel = next_token(&p);
    *out_thread = next_token(&p);
    return 0;
}

static int parse_and_get_team(server_t *server, client_t *client,
    char *team_str, team_t **out_team)
{
    uuid_t team_uuid;
    team_t *team = NULL;

    if (uuid_parse(team_str, team_uuid) == 0) {
        team = find_team_by_uuid(server, team_uuid);
    } else {
        team = find_team_by_name(server, team_str);
    }
    if (!team) {
        send_response(client, CODE_NOT_FOUND, "Team not found.");
        return -1;
    }
    *out_team = team;
    return 0;
}

static int parse_and_get_channel(server_t *server, client_t *client,
    char *channel_str, channel_t **out_channel)
{
    uuid_t channel_uuid;
    channel_t *channel = NULL;

    if (uuid_parse(channel_str, channel_uuid) == 0) {
        channel = find_channel_by_uuid(server, channel_uuid);
    } else {
        channel = find_channel_by_name(server, channel_str);
    }
    if (!channel) {
        send_response(client, CODE_CHANNEL_NOT_FOUND, "Channel not found.");
        return -1;
    }
    *out_channel = channel;
    return 0;
}

static int parse_and_get_thread(server_t *server, client_t *client,
    char *thread_str, thread_t **out_thread)
{
    uuid_t thread_uuid;
    thread_t *thread = NULL;

    if (uuid_parse(thread_str, thread_uuid) == 0) {
        thread = find_thread_by_uuid(server, thread_uuid);
    } else {
        thread = find_thread_by_title(server, thread_str);
    }
    if (!thread) {
        send_response(client, CODE_THREAD_NOT_FOUND, "Thread not found.");
        return -1;
    }
    *out_thread = thread;
    return 0;
}

static int parse_and_validate_channel(use_context_t *ctx,
    char *channel_str, channel_t **out_channel)
{
    channel_t *channel = NULL;

    if (parse_and_get_channel(ctx->server, ctx->client, channel_str,
            out_channel) != 0)
        return -1;
    channel = *out_channel;
    if (uuid_compare(channel->team_uuid, ctx->team->uuid) != 0) {
        send_response(ctx->client, CODE_CHANNEL_NOT_FOUND,
            "Channel not found.");
        return -1;
    }
    return 0;
}

static int parse_and_validate_thread(use_context_t *ctx,
    channel_t *channel, char *thread_str)
{
    thread_t *thread = NULL;

    if (parse_and_get_thread(ctx->server, ctx->client, thread_str,
            &thread) != 0)
        return -1;
    if (uuid_compare(thread->channel_uuid, channel->uuid) != 0) {
        send_response(ctx->client, CODE_THREAD_NOT_FOUND,
            "Thread not found.");
        return -1;
    }
    return 0;
}

static int handle_channel_and_thread(use_context_t *ctx,
    char *channel_str, char *thread_str)
{
    channel_t *channel = NULL;

    if (parse_and_validate_channel(ctx, channel_str, &channel) != 0)
        return -1;
    if (!thread_str)
        return 0;
    if (parse_and_validate_thread(ctx, channel, thread_str) != 0)
        return -1;
    return 0;
}

void cmd_use(server_t *server, client_t *client, char *args)
{
    char *team_str = NULL;
    char *channel_str = NULL;
    char *thread_str = NULL;
    use_context_t ctx;

    if (parse_use_args(args, &team_str, &channel_str, &thread_str) == -1) {
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "Syntax error in parameters.");
        return;
    }
    if (parse_and_get_team(server, client, team_str, &ctx.team) != 0)
        return;
    ctx.server = server;
    ctx.client = client;
    if (channel_str) {
        if (handle_channel_and_thread(&ctx, channel_str, thread_str) != 0)
            return;
    }
    send_response(client, CODE_COMMAND_OK, "Context set.");
}
