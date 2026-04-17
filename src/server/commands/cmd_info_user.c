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
    char *msg = "Request failed, user doesn't exist";

    if (user == NULL) {
        send_response(client, CODE_NOT_LOGGED_IN, msg);
        return;
    }
    uuid_to_string(user->uuid, uuid_buf);
    snprintf(result, sizeof(result),
        "User %s is logged Successfully.\nUid : %s", user->name, uuid_buf);
    send_response(client, CODE_COMMAND_OK, result);
}
