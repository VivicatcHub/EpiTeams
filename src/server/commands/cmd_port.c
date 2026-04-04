/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_port.c
*/

#include "my_teams.h"

void cmd_port(server_t *server UNUSED, client_t *client, char *args)
{
    args_parser_t p = { 0 };

    if (!args || sscanf(args, "%d,%d,%d,%d,%d,%d",
            &p.h1, &p.h2, &p.h3, &p.h4, &p.p1, &p.p2) != 6) {
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "Syntax error in parameters.");
        return;
    }
    if (client->data_socket != -1) {
        close(client->data_socket);
        client->data_socket = -1;
    }
    memset(&client->data_addr, 0, sizeof(client->data_addr));
    client->data_addr.sin_family = AF_INET;
    client->data_addr.sin_port = htons((p.p1 << 8) | p.p2);
    client->data_addr.sin_addr.s_addr =
        htonl(((unsigned)p.h1 << 24) | ((unsigned)p.h2 << 16)
        | ((unsigned)p.h3 << 8) | (unsigned)p.h4);
    client->mode = ACTIVE;
    send_response(client, CODE_COMMAND_OK, "PORT command successful.");
}
