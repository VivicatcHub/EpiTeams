/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_user.c
*/

#include "my_teams.h"

static int check_arg(client_t *client, char *args)
{
    if (!args || args[0] == '\0') {
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "Syntax error in parameters.");
        return -1;
    }
    return 0;
}

static int is_user_connected(server_t *server, char const *name)
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

static void send_user_info(client_t *client, user_t *user, int online)
{
    char uuid_buf[UUID_STR_LEN];
    char buf[MAX_BUFFER];

    uuid_to_string(user->uuid, uuid_buf);
    snprintf(buf, MAX_BUFFER, "%s (%s) -> %s", user->name, uuid_buf,
        online ? "ONLINE" : "OFFLINE");
    send_response(client, CODE_COMMAND_OK, buf);
    send_response(client, CODE_COMMAND_END, "End of user info.");
}

static int resolve_user_arg(server_t *server, char *args, uuid_t target)
{
    user_t *name;

    if (string_to_uuid(args, target) == 0)
        return 0;
    name = find_user_by_name(server, args);
    if (!name)
        return -1;
    memcpy(target, name->uuid, sizeof(uuid_t));
    return 0;
}

static void send_404(client_t *client, uuid_t uuid)
{
    char uuid_buf[UUID_STR_LEN];
    char buf[MAX_BUFFER];

    uuid_to_string(uuid, uuid_buf);
    snprintf(buf, MAX_BUFFER, "User not found (%s).", uuid_buf);
    send_response(client, CODE_NOT_FOUND, buf);
}

void cmd_user(server_t *server UNUSED, client_t *client, char *args)
{
    user_t *user;
    uuid_t target;
    int online;

    if (check_arg(client, args) == -1)
        return;
    if (resolve_user_arg(server, args, target) != 0) {
        send_response(client, CODE_SYNTAX_ERROR_PARAM, "Invalid UUID.");
        return;
    }
    TAILQ_FOREACH(user, &server->data.users, entries) {
        if (uuid_compare(user->uuid, target) == 0) {
            online = is_user_connected(server, user->name);
            send_user_info(client, user, online);
            return;
        }
    }
    send_404(client, target);
}
