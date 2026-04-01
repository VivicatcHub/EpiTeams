/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** handle_client_data.c
*/

#include "my_teams.h"

static int handle_full_buffer(server_t *server, client_t *client)
{
    if (strstr(client->buffer, CRLF) != NULL)
        return 0;
    send_response(client, CODE_SYNTAX_ERROR_PARAM,
        "Command too long (missing CRLF).");
    remove_client(server, client);
    return -1;
}

static int read_from_client(client_t *client)
{
    int bytes;

    if (client->buffer_position >= MAX_BUFFER - 1)
        return 0;
    bytes = read(client->socket,
        client->buffer + client->buffer_position,
        MAX_BUFFER - client->buffer_position - 1);
    if (bytes <= 0)
        return -1;
    client->buffer_position += bytes;
    client->buffer[client->buffer_position] = '\0';
    return 0;
}

static void crlf_search(server_t *server, client_t *client)
{
    char *crlf = strstr(client->buffer, CRLF);
    char command[MAX_BUFFER];
    int cmd_len;

    while (crlf != NULL) {
        cmd_len = crlf - client->buffer;
        memcpy(command, client->buffer, cmd_len);
        command[cmd_len] = '\0';
        memmove(client->buffer, crlf + 2,
            client->buffer_position - cmd_len - 2);
        client->buffer_position -= cmd_len + 2;
        client->buffer[client->buffer_position] = '\0';
        dispatch_command(server, client, command);
        crlf = strstr(client->buffer, CRLF);
    }
}

void handle_client_data(server_t *server, client_t *client)
{
    if (read_from_client(client) == -1) {
        remove_client(server, client);
        return;
    }
    if (client->buffer_position >= MAX_BUFFER - 1 &&
        handle_full_buffer(server, client) == -1)
        return;
    crlf_search(server, client);
}
