/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** remove_client.c
*/

#include "my_teams.h"
#include "logging_server.h"

static void disconnect_client(client_t *client)
{
    printf("Client disconnected: %s:%d\n",
        get_ip_address(&client->address.sin_addr),
        get_port(&client->address.sin_port));
    destroy_client(client);
}

void remove_client(server_t *server, client_t *client)
{
    char uuid_buf[UUID_STR_LEN];

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (server->clients[i] != client)
            continue;
        if (client->username) {
            uuid_to_string(find_user_by_name(server, client->username)->uuid,
                uuid_buf);
            server_event_user_logged_out(uuid_buf);
        }
        disconnect_client(client);
        server->clients[i] = NULL;
        return;
    }
}
