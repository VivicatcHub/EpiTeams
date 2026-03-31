/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** destroy_client.c
*/

#include "my_teams.h"

void destroy_client(client_t *client)
{
    if (!client)
        return;
    if (client->socket != -1)
        close(client->socket);
    if (client->data_socket != -1)
        close(client->data_socket);
    free(client->username);
    free(client);
}
