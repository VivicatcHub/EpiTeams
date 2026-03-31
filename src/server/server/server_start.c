/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** server_start.c
*/

#include "my_teams.h"

static int resolve_root_path(char *root_path, char *resolved)
{
    if (!realpath(root_path, resolved)) {
        fprintf(stderr, "Error: Failed to resolve root path\n");
        return -1;
    }
    return 0;
}

int start_server(char *port_str, char *root_path)
{
    server_t server;
    char resolved[PATH_MAX];

    server.port = atoi(port_str);
    if (resolve_root_path(root_path, resolved) == -1)
        return 84;
    server.root_path = resolved;
    server.socket = -1;
    server.state = RUNNING;
    for (int i = 0; i < MAX_CLIENTS; i++)
        server.clients[i] = NULL;
    printf("Starting FTP server on port %d\n", server.port);
    printf("Home directory: %s\n", server.root_path);
    if (init_server(&server) == -1) {
        fprintf(stderr, "Error: Failed to create server\n");
        return 84;
    }
    run_server(&server);
    cleanup_server(&server);
    return 0;
}
