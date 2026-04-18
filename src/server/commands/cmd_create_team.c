/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_create_team.c
*/

#include "my_teams.h"
#include "logging_server.h"

static int team_name_exists(server_t *server, char const *name)
{
    return find_team_by_name(server, name) != NULL;
}

static int parse_create_team_args(char *args, char **out_name,
    char **out_desc)
{
    char *space = NULL;

    if (!args || args[0] == '\0')
        return -1;
    space = strchr(args, ' ');
    if (!space)
        return -1;
    *space = '\0';
    *out_name = args;
    *out_desc = space + 1;
    return 0;
}

static team_t *create_and_store_team(server_t *server, char *name,
    char *desc)
{
    team_t *team = NULL;

    team = malloc(sizeof(team_t));
    if (!team)
        return NULL;
    uuid_generate(team->uuid);
    strncpy(team->name, name, MAX_NAME_LENGTH - 1);
    team->name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(team->description, desc, MAX_DESCRIPTION_LENGTH - 1);
    team->description[MAX_DESCRIPTION_LENGTH - 1] = '\0';
    TAILQ_INIT(&team->members);
    TAILQ_INIT(&team->channels);
    team->created_at = time(NULL);
    TAILQ_INSERT_TAIL(&server->data.teams, team, entries);
    return team;
}

static void notify_team_created_connected_users(server_t *server, team_t *team)
{
    char buf[MAX_BUFFER];
    char team_uuid_str[UUID_STR_LEN];

    uuid_to_string(team->uuid, team_uuid_str);
    snprintf(buf, sizeof(buf), "%s \"%s\" \"%s\"", team_uuid_str,
        team->name, team->description);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!server->clients[i] || !server->clients[i]->username)
            continue;
        send_response(server->clients[i], CODE_TEAM_CREATED_EVENT, buf);
    }
}

static void notify_team_created(server_t *server, team_t *team,
    user_t *creator, client_t *client)
{
    char team_uuid_str[UUID_STR_LEN];
    char user_uuid_str[UUID_STR_LEN];
    char buf[MAX_BUFFER];

    uuid_to_string(team->uuid, team_uuid_str);
    uuid_to_string(creator->uuid, user_uuid_str);
    server_event_team_created(team_uuid_str, team->name, user_uuid_str);
    notify_team_created_connected_users(server, team);
    snprintf(buf, sizeof(buf), "%s (%s) - %s", team->name, team_uuid_str,
        team->description);
    send_response(client, CODE_COMMAND_OK, buf);
}

static int create_and_publish_team(server_t *server, client_t *client,
    team_create_exec_t *params)
{
    team_t *team = NULL;

    team = create_and_store_team(server, params->name, params->desc);
    if (!team)
        return -1;
    save_teams(&server->data.teams);
    notify_team_created(server, team, params->creator, client);
    return 0;
}

static int validate_team_creation(server_t *server, client_t *client,
    char *name, user_t **creator)
{
    if (is_user_logged_in(server, client) == 0)
        return -1;
    if (team_name_exists(server, name)) {
        send_response(client, CODE_ALREADY_SUBSCRIBED,
            "Team already exists.");
        return -1;
    }
    *creator = find_user_by_name(server, client->username);
    if (!*creator)
        return -1;
    return 0;
}

static void process_create_team(server_t *server, client_t *client,
    char *name, char *desc)
{
    user_t *creator = NULL;
    team_create_exec_t params;

    if (validate_team_creation(server, client, name, &creator) != 0)
        return;
    params.creator = creator;
    params.name = name;
    params.desc = desc;
    create_and_publish_team(server, client, &params);
}

void cmd_create_team(server_t *server, client_t *client, char *args)
{
    char *name = NULL;
    char *desc = NULL;

    if (parse_create_team_args(args, &name, &desc) == -1) {
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "Syntax error in parameters.");
        return;
    }
    process_create_team(server, client, name, desc);
}
