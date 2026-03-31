/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** handle_client_data.c
*/

#include "my_teams.h"

static int read_from_client(client_t *client)
{
    int bytes = read(client->socket,
        client->buffer + client->buffer_position,
        MAX_BUFFER - client->buffer_position - 1);

    if (bytes <= 0) {
        return -1;
    }
    client->buffer_position += bytes;
    client->buffer[client->buffer_position] = '\0';
    return 0;
}

void handle_client_data(server_t *server, client_t *client)
{
    char *crlf;
    char command[MAX_BUFFER];
    int cmd_len;

    if (read_from_client(client) == -1) {
        remove_client(server, client);
        return;
    }
    crlf = strstr(client->buffer, CRLF);
    while (crlf != NULL) {
        cmd_len = crlf - client->buffer;
        memcpy(command, client->buffer, cmd_len);
        command[cmd_len] = '\0';
        memmove(client->buffer, crlf + 2,
            client->buffer_position - cmd_len - 2);
        client->buffer_position -= cmd_len + 2;
        client->buffer[client->buffer_position] = '\0';
        dispatch_command(client, command);
        crlf = strstr(client->buffer, CRLF);
    }
}
