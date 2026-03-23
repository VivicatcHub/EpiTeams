/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** server_cleanup.c
*/

#include "my_teams.h"

void cleanup_server(server_t *server)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (server->clients[i]) {
            destroy_client(server->clients[i]);
            server->clients[i] = NULL;
        }
    }
    if (server->socket != -1) {
        close(server->socket);
        server->socket = -1;
    }
    printf("\nShutting down server...\n");
}
