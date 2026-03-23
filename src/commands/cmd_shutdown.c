/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_shutdown.c
*/

#include "my_teams.h"

void cmd_shutdown(client_t *client, char *args UNUSED)
{
    server_t *server = (server_t *)client->server;

    if (client->permission != ADMIN) {
        send_response(client, CODE_NOT_LOGGED_IN,
            "Permission denied.");
        return;
    }
    send_response(client, CODE_QUIT,
        "Server shutting down.");
    server->state = STOPPING;
}
