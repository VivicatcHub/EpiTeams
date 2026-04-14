/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_create_channel.c
*/

#include "my_teams.h"
#include "logging_server.h"

static int parse_create_channel_args(char *args, char **out_team,
    char **out_name, char **out_desc)
{
    char *s1 = NULL;
    char *s2 = NULL;

    if (!args || args[0] == '\0')
        return -1;
    s1 = strchr(args, ' ');
    if (!s1)
        return -1;
    *s1 = '\0';
    s2 = strchr(s1 + 1, ' ');
    if (!s2)
        return -1;
    *s2 = '\0';
    *out_team = args;
    *out_name = s1 + 1;
    *out_desc = s2 + 1;
    return 0;
}

static channel_t *create_and_store_channel(server_t *server, uuid_t team_uuid,
    char *name, char *desc)
{
    channel_t *channel = NULL;

    channel = malloc(sizeof(channel_t));
    if (!channel)
        return NULL;
    uuid_generate(channel->uuid);
    memcpy(channel->team_uuid, team_uuid, sizeof(uuid_t));
    strncpy(channel->name, name, MAX_NAME_LENGTH - 1);
    channel->name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(channel->description, desc, MAX_DESCRIPTION_LENGTH - 1);
    channel->description[MAX_DESCRIPTION_LENGTH - 1] = '\0';
    TAILQ_INIT(&channel->threads);
    channel->created_at = time(NULL);
    TAILQ_INSERT_TAIL(&server->data.channels, channel, entries);
    return channel;
}

static void notify_channel_created(channel_t *channel, uuid_t team_uuid,
    client_t *client)
{
    char team_uuid_str[UUID_STR_LEN];
    char channel_uuid_str[UUID_STR_LEN];

    uuid_to_string(team_uuid, team_uuid_str);
    uuid_to_string(channel->uuid, channel_uuid_str);
    server_event_channel_created(team_uuid_str, channel_uuid_str,
        channel->name);
    send_response(client, CODE_COMMAND_OK, "Channel created.");
    send_response(client, CODE_COMMAND_END, "End of channel creation.");
}

static int process_create_channel_tokens(server_t *server,
    client_t *client, channel_create_params_t *p)
{
    uuid_t team_uuid;
    team_t *team = NULL;
    channel_t *channel = NULL;

    if (string_to_uuid(p->team_str, team_uuid) != 0)
        team = find_team_by_name(server, p->team_str);
    else
        team = find_team_by_uuid(server, team_uuid);
    if (!team) {
        send_response(client, CODE_NOT_FOUND, "Team not found.");
        return -1;
    }
    channel = create_and_store_channel(server, team->uuid, p->name, p->desc);
    if (!channel)
        return -1;
    save_channels(&server->data.channels);
    notify_channel_created(channel, team->uuid, client);
    return 0;
}

void cmd_create_channel(server_t *server, client_t *client, char *args)
{
    char *team_str = NULL;
    char *name = NULL;
    char *desc = NULL;
    channel_create_params_t p;

    if (parse_create_channel_args(args, &team_str, &name, &desc) == -1) {
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "Syntax error in parameters.");
        return;
    }
    p.team_str = team_str;
    p.name = name;
    p.desc = desc;
    process_create_channel_tokens(server, client, &p);
}
