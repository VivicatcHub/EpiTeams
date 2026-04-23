/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_info.c
*/

#include "my_teams.h"

static user_t *find_user_from_arg(server_t *server, char *args)
{
    user_t *user = NULL;
    uuid_t uuid;

    if (!args || args[0] == '\0')
        return NULL;
    user = find_user_by_name(server, args);
    if (user)
        return user;
    if (string_to_uuid(args, uuid) == 0)
        return find_user_by_uuid(server, uuid);
    return NULL;
}

static user_t *get_current_user_or_respond(server_t *server, client_t *client)
{
    user_t *user = NULL;

    if (!client->username) {
        send_response(client, CODE_NOT_LOGGED_IN,
            "Request failed, user doesn't exist");
        return NULL;
    }
    user = find_user_by_name(server, client->username);
    if (!user) {
        send_response(client, CODE_NOT_LOGGED_IN,
            "Request failed, user doesn't exist");
        return NULL;
    }
    return user;
}

static void send_user_info_response(client_t *client, user_t *user, int online)
{
    char uuid_buf[UUID_STR_LEN];
    char result[MAX_BUFFER];

    uuid_to_string(user->uuid, uuid_buf);
    snprintf(result, sizeof(result), "%s (%s) -> %s",
        user->name, uuid_buf, online ? "ONLINE" : "OFFLINE");
    send_response(client, CODE_COMMAND_OK, result);
}

void cmd_info(server_t *server, client_t *client, char *args)
{
    user_t *user = NULL;

    if (args && args[0] != '\0') {
        user = find_user_from_arg(server, args);
        if (!user) {
            send_response(client, CODE_NOT_FOUND, "User not found.");
            return;
        }
    } else {
        user = get_current_user_or_respond(server, client);
        if (!user)
            return;
    }
    send_user_info_response(client, user,
        is_username_connected(server, user->name));
}
