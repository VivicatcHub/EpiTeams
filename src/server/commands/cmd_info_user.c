/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_info.c
*/

#include "my_teams.h"

void cmd_info(server_t *server, client_t *client, char *args)
{
    char name[MAX_NAME_LENGTH];
    char result[MAX_BUFFER];
    char uuid_buf[UUID_STR_LEN];
    user_t *user = find_user_by_name(server, client->username);
    char *msg = "Request failed, user is not logged in";

    if (user == NULL) {
        send_response(client, CODE_NOT_LOGGED_IN, msg);
        return;
    }
    uuid_to_string(user->uuid, uuid_buf);
    if (ONLINE == user->status) {
        snprintf(result, sizeof(result),
            "User %s with uuid %s is connected", client->username, uuid_buf);
        send_response(client, CODE_COMMAND_OK, result);
    }
    snprintf(result, sizeof(result),
        "User %s with uuid %s is not connected",
        client->username, uuid_buf);
    send_response(client, CODE_NOT_LOGGED_IN, result);
}
