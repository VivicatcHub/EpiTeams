/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** create_client.c
*/

#include "my_teams.h"

client_t *create_client(int socket, struct sockaddr_in *address)
{
    client_t *client = malloc(sizeof(client_t));

    if (!client)
        return NULL;
    client->socket = socket;
    client->address = *address;
    client->state = CONNECTED;
    client->mode = NONE;
    client->permission = USER;
    client->data_socket = -1;
    client->child_process_pid = 0;
    client->username = NULL;
    client->current_dir = NULL;
    client->buffer_position = 0;
    memset(client->buffer, 0, MAX_BUFFER);
    return client;
}
