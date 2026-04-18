/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_info.c
*/

#include "my_teams.h"

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

void cmd_info(server_t *server, client_t *client, char *args UNUSED)
{
    char buf[MAX_BUFFER];
    char uuid_buf[UUID_STR_LEN];
    user_t *user = find_user_by_name(server, client->username);
    int online = 0;
    char *msg = "Request failed, user doesn't exist";

    if (user == NULL) {
        send_response(client, CODE_NOT_LOGGED_IN, msg);
        return;
    }
    online = is_user_connected(server, user->name);
    uuid_to_string(user->uuid, uuid_buf);
    snprintf(buf, MAX_BUFFER, "%s (%s) -> %s", user->name, uuid_buf,
        online ? "ONLINE" : "OFFLINE");
    send_response(client, CODE_COMMAND_BEGIN, buf);
    send_response(client, CODE_COMMAND_END, "No more user info.");
}
