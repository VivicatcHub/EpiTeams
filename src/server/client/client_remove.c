/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** remove_client.c
*/

#include "my_teams.h"


static void disconnect_client(client_t *client)
{
    printf("Client disconnected: %s:%d\n",
        get_ip_address(&client->address.sin_addr),
        get_port(&client->address.sin_port));
    destroy_client(client);
}

void remove_client(server_t *server, client_t *client)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (server->clients[i] != client)
            continue;
        if (client->username)
            sync_user_connection_state(server, client->username, 0);
        disconnect_client(client);
        server->clients[i] = NULL;
        return;
    }
}
