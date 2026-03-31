/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** accept_client.c
*/

#include "my_teams.h"

int add_client(server_t *server, int fd, struct sockaddr_in *addr)
{
    client_t *client;
    int i;

    for (i = 0; i < MAX_CLIENTS; i++) {
        if (server->clients[i] == NULL)
            break;
    }
    if (i == MAX_CLIENTS)
        return -1;
    client = create_client(fd, addr);
    if (!client)
        return -1;
    server->clients[i] = client;
    printf("New client connected: %s:%d\n",
        get_ip_address(&addr->sin_addr), get_port(&addr->sin_port));
    send_response(client, CODE_SERVICE_READY, "Service ready for new user.");
    return 0;
}
