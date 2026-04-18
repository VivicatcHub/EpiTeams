/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_subscribed.c
*/

#include "my_teams.h"

static int is_name_connected(server_t *server, char const *name)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!server->clients[i])
            continue;
        if (!server->clients[i]->username)
            continue;
        if (strcmp(server->clients[i]->username, name) == 0)
            return 1;
    }
    return 0;
}

static int resolve_team(server_t *server, client_t *client, char *team_str,
    uuid_t out)
{
    team_t *team = NULL;

    if (string_to_uuid(team_str, out) == 0) {
        team = find_team_by_uuid(server, out);
    } else {
        team = find_team_by_name(server, team_str);
        if (team)
            memcpy(out, team->uuid, sizeof(uuid_t));
    }
    if (!team) {
        send_response(client, CODE_NOT_FOUND, "Team not found.");
        return -1;
    }
    return 0;
}

static void send_team_info(client_t *client, team_t *team)
{
    char buf[MAX_BUFFER];
    char uuid_buf[UUID_STR_LEN];

    uuid_to_string(team->uuid, uuid_buf);
    snprintf(buf, MAX_BUFFER, "%s (%s) - %s", team->name, uuid_buf,
        team->description);
    send_response(client, CODE_COMMAND_BEGIN, buf);
}

static void send_user_sub_info(server_t *server, client_t *client,
    user_t *user)
{
    char buf[MAX_BUFFER];
    char uuid_buf[UUID_STR_LEN];
    const char *status = "OFFLINE";

    uuid_to_string(user->uuid, uuid_buf);
    if (is_name_connected(server, user->name) == 1)
        status = "ONLINE";
    snprintf(buf, MAX_BUFFER, "%s (%s) -> %s", user->name, uuid_buf, status);
    send_response(client, CODE_COMMAND_OK_WITH_INFO, buf);
}

static void list_subscribed_teams(server_t *server, client_t *client,
    user_t *user)
{
    team_subscription_t *sub = NULL;
    team_t *team = NULL;

    TAILQ_FOREACH(sub, &user->subscribed_teams, entries) {
        team = find_team_by_uuid(server, sub->team_uuid);
        if (!team)
            continue;
        send_team_info(client, team);
    }
    send_response(client, CODE_COMMAND_END, "No more subscribed teams.");
}

static int send_if_user_subscribed(server_t *server, client_t *client,
    user_t *user, uuid_t target)
{
    team_subscription_t *sub = NULL;

    TAILQ_FOREACH(sub, &user->subscribed_teams, entries) {
        if (uuid_compare(sub->team_uuid, target) == 0) {
            send_user_sub_info(server, client, user);
            return 1;
        }
    }
    return 0;
}

static void list_team_subscribers(server_t *server, client_t *client,
    uuid_t target)
{
    user_t *u = NULL;

    TAILQ_FOREACH(u, &server->data.users, entries) {
        if (send_if_user_subscribed(server, client, u, target))
            continue;
    }
    send_response(client, CODE_COMMAND_END, "No more team subscribers.");
}

void cmd_subscribed(server_t *server, client_t *client, char *args)
{
    user_t *user = find_user_by_name(server, client->username);
    uuid_t target;

    if (!user) {
        send_response(client, CODE_NOT_LOGGED_IN, "Not logged in.");
        return;
    }
    if (!args || args[0] == '\0') {
        list_subscribed_teams(server, client, user);
        return;
    }
    if (resolve_team(server, client, args, target) != 0)
        return;
    list_team_subscribers(server, client, target);
}
