/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** destroy_client.c
*/

#include "my_teams.h"

void destroy_pending(client_t *client)
{
    pending_command_t *pending;

    while (client->pending_commands_head) {
        pending = client->pending_commands_head;
        client->pending_commands_head = pending->next;
        free(pending->line);
        free(pending);
    }
}

void destroy_client(client_t *client)
{
    if (!client)
        return;
    destroy_pending(client);
    if (client->socket != -1)
        close(client->socket);
    if (client->data_socket != -1)
        close(client->data_socket);
    if (client->username)
        free(client->username);
    free(client);
}
