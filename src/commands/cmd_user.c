/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_user.c
*/

#include "my_teams.h"

void cmd_user(client_t *client, char *args)
{
    if (!args || args[0] == '\0') {
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "Syntax error in parameters.");
        return;
    }
    if (args) {
        free(client->username);
        client->username = strdup(args);
        client->state = USER_PROVIDED;
        send_response(client, CODE_PASSWORD_REQUIRED,
            "User name okay, need password.");
        return;
    }
}
