/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_create_team.c
*/

#include "my_teams.h"
#include "logging_server.h"

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

static void notify_team_created(team_t *team, user_t *creator,
    client_t *client)
{
    char team_uuid_str[UUID_STR_LEN];
    char user_uuid_str[UUID_STR_LEN];

    uuid_to_string(team->uuid, team_uuid_str);
    uuid_to_string(creator->uuid, user_uuid_str);
    server_event_team_created(team_uuid_str, team->name, user_uuid_str);
    send_response(client, CODE_COMMAND_OK, "Team created.");
}

void cmd_create_team(server_t *server, client_t *client, char *args)
{
    char *name = NULL;
    char *desc = NULL;
    team_t *team = NULL;
    user_t *creator = NULL;

    if (parse_create_team_args(args, &name, &desc) == -1) {
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "Syntax error in parameters.");
        return;
    }
    creator = find_user_by_name(server, client->username);
    if (!creator) {
        send_response(client, CODE_NOT_LOGGED_IN, "Not logged in.");
        return;
    }
    team = create_and_store_team(server, name, desc);
    if (!team)
        return;
    save_teams(&server->data.teams);
    notify_team_created(team, creator, client);
}
