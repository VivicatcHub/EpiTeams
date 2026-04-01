/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_user.c
*/

#include "my_teams.h"

void cmd_user(server_t *server, client_t *client, char *args)
{
    (void)server;
    if (!args || args[0] == '\0') {
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "Syntax error in parameters.");
        return;
    }
    if (args) {
        client->state = CONNECTED;
        send_response(client, CODE_LOGIN_SUCCESS,
            "User logged in successfully.");
    }
}
