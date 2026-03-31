/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** open_data_conn.c
*/

#include "my_teams.h"

int open_data_conn(client_t *client)
{
    int data_fd;

    if (client->mode == PASSIVE) {
        data_fd = accept(client->data_socket, NULL, NULL);
        return data_fd;
    }
    if (client->mode == ACTIVE) {
        data_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (data_fd < 0)
            return -1;
        if (connect(data_fd, (struct sockaddr *)&client->data_addr,
                sizeof(client->data_addr)) < 0) {
            close(data_fd);
            return -1;
        }
        return data_fd;
    }
    return -1;
}
