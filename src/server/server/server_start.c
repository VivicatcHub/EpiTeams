/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** server_start.c
*/

#include "my_teams.h"

int start_server(char *port_str)
{
    server_t server;

    server.port = atoi(port_str);
    server.socket = -1;
    server.state = RUNNING;
    for (int i = 0; i < MAX_CLIENTS; i++)
        server.clients[i] = NULL;
    printf("Starting myteams server on port %d\n", server.port);
    if (init_server(&server) == -1) {
        fprintf(stderr, "Error: Failed to create server\n");
        return EXIT_ERROR;
    }
    run_server(&server);
    cleanup_server(&server);
    return 0;
}
