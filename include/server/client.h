/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** client.h
*/

#ifndef CLIENT_H
    #define CLIENT_H

    #define MAX_BUFFER 4096

typedef struct server_s server_t;

enum client_state {
    CONNECTED,
    AUTHENTICATED
};

enum client_permission {
    USER,
    ADMIN
};

enum client_mode {
    NONE,
    ACTIVE,
    PASSIVE
};

typedef struct client_s {
    int socket;
    struct sockaddr_in address;
    enum client_state state;
    enum client_mode mode;
    enum client_permission permission;
    char *username;
    char buffer[MAX_BUFFER];
    int buffer_position;
    int data_socket;
    struct sockaddr_in data_addr;
} client_t;

void accept_client(server_t *server);
void remove_client(server_t *server, client_t *client);
int add_client(server_t *server, int fd, struct sockaddr_in *addr);
void handle_client_data(server_t *server, client_t *client);
client_t *create_client(int socket, struct sockaddr_in *address);
void destroy_client(client_t *client);
void send_response(client_t *client, char const *code, char const *msg);

#endif
