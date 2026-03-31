/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** parent_cleanup_data.c
*/

#include "my_teams.h"

void parent_cleanup_data(client_t *client)
{
    if (client->data_socket != -1) {
        close(client->data_socket);
        client->data_socket = -1;
    }
    client->mode = NONE;
}
