/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** server_init.c
*/

#include "my_teams.h"

static void set_server_socket(server_t *server)
{
    int opt = 1;

    server->socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server->socket == -1) {
        perror("socket");
        exit(EXIT_ERROR);
    }
    if (setsockopt(server->socket,
            SOL_SOCKET,
            SO_REUSEADDR,
            &opt,
            sizeof(opt)) == -1) {
        perror("setsockopt");
        close(server->socket);
        exit(EXIT_ERROR);
    }
}

static void bind_server(server_t *server)
{
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(server->port);
    if (bind(server->socket,
            (struct sockaddr *)&server_addr,
            sizeof(server_addr)) == -1) {
        perror("bind");
        close(server->socket);
        exit(EXIT_ERROR);
    }
}

int init_server(server_t *server)
{
    set_server_socket(server);
    bind_server(server);
    if (listen(server->socket, MAX_CLIENTS) == -1) {
        perror("listen");
        close(server->socket);
        return -1;
    }
    return 0;
}
