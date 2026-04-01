/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** server.h
*/

#ifndef SERVER_H
    #define SERVER_H

    #include "client.h"
    #include "data_structures.h"

    #define MAX_CLIENTS 2048

enum server_state {
    RUNNING,
    STOPPING
};

typedef struct server_s {
    int port;
    int socket;
    enum server_state state;
    client_t *clients[MAX_CLIENTS];
    server_data_t data;
} server_t;

int start_server(char *port_str);
int init_server(server_t *server);
void run_server(server_t *server);
void cleanup_server(server_t *server);
void handle_server_events(server_t *server);
#endif
