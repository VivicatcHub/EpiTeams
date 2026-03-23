/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** server.h
*/

#ifndef SERVER_H
    #define SERVER_H

    #include "client.h"

    #define MAX_CLIENTS 2048
    #define PATH_MAX 4096

enum server_state {
    RUNNING,
    STOPPING
};

typedef struct server_s {
    int port;
    char *root_path;
    int socket;
    enum server_state state;
    client_t *clients[MAX_CLIENTS];
} server_t;

int start_server(char *port_str, char *root_path);
int init_server(server_t *server);
void run_server(server_t *server);
void cleanup_server(server_t *server);
void handle_server_events(server_t *server);
#endif
