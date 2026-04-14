/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_unsubscribe.c
*/

#include "my_teams.h"
#include "logging_server.h"

static int check_arg(client_t *client, char *args)
{
    if (!args || args[0] == '\0') {
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "Syntax error in parameters.");
        return -1;
    }
    return 0;
}

static int resolve_team(server_t *server, client_t *client,
    char *team_str, team_t **out)
{
    uuid_t team_uuid;
    team_t *team = NULL;

    if (string_to_uuid(team_str, team_uuid) == 0) {
        team = find_team_by_uuid(server, team_uuid);
    } else {
        team = find_team_by_name(server, team_str);
    }
    if (!team) {
        send_response(client, CODE_NOT_FOUND, "Team not found.");
        return -1;
    }
    *out = team;
    return 0;
}

static team_subscription_t *find_subscription(user_t *user, team_t *team)
{
    team_subscription_t *sub = NULL;

    TAILQ_FOREACH(sub, &user->subscribed_teams, entries) {
        if (uuid_compare(sub->team_uuid, team->uuid) == 0)
            return sub;
    }
    return NULL;
}

static void remove_subscription(user_t *user, team_subscription_t *sub)
{
    TAILQ_REMOVE(&user->subscribed_teams, sub, entries);
    free(sub);
}

static user_t *get_authenticated_user(server_t *server, client_t *client)
{
    user_t *user = find_user_by_name(server, client->username);

    if (!user)
        send_response(client, CODE_NOT_LOGGED_IN, "Not logged in.");
    return user;
}

static int do_unsubscribe(client_t *client, user_t *user, team_t *team)
{
    team_subscription_t *sub = find_subscription(user, team);
    char team_uuid_buf[UUID_STR_LEN];
    char user_uuid_buf[UUID_STR_LEN];

    if (!sub) {
        send_response(client, CODE_NOT_FOUND, "Not subscribed.");
        return -1;
    }
    remove_subscription(user, sub);
    uuid_to_string(team->uuid, team_uuid_buf);
    uuid_to_string(user->uuid, user_uuid_buf);
    server_event_user_unsubscribed(team_uuid_buf, user_uuid_buf);
    send_response(client, CODE_COMMAND_OK, "Unsubscribed.");
    return 0;
}

void cmd_unsubscribe(server_t *server, client_t *client, char *args)
{
    char *team_str = NULL;
    team_t *team = NULL;
    user_t *user = NULL;

    if (check_arg(client, args) == -1)
        return;
    team_str = args;
    user = get_authenticated_user(server, client);
    if (!user)
        return;
    if (resolve_team(server, client, team_str, &team) != 0)
        return;
    do_unsubscribe(client, user, team);
}
