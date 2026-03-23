/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_pass.c
*/

#include "my_teams.h"

const user_t users[] = {
    {"Anonymous", ""},
    {"admin", "admin123"},
    {"LORAY", "ValentinMyBeloved"},
    {NULL, NULL}
};

static void verify_password(client_t *client, char *password)
{
    for (int i = 0; users[i].username != NULL; i++) {
        if (strcmp(client->username, users[i].username) != 0)
            continue;
        if (strcmp(password, users[i].password) != 0)
            break;
        send_response(client, CODE_LOGIN_SUCCESS,
            "User logged in, proceed.");
        client->state = AUTHENTICATED;
        if (strcmp(client->username, "admin") == 0)
            client->permission = ADMIN;
        return;
    }
    send_response(client, CODE_NOT_LOGGED_IN,
        "Incorrect password.");
}

void cmd_pass(client_t *client, char *args)
{
    if (client->state != USER_PROVIDED) {
        send_response(client, CODE_BAD_SEQUENCE,
            "Login with USER first.");
        return;
    }
    if (!args) {
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "Syntax error in parameters.");
        return;
    }
    verify_password(client, args);
}
