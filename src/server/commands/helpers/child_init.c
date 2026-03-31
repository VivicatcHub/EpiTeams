/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** child_init.c
*/

#include "my_teams.h"

void child_init(client_t *client)
{
    server_t *server = (server_t *)client->server;

    signal(SIGINT, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
    signal(SIGPIPE, SIG_IGN);
    if (server->socket != -1)
        close(server->socket);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (server->clients[i] && server->clients[i] != client)
            close(server->clients[i]->socket);
    }
}
