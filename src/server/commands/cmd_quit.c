/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_quit.c
*/

#include "my_teams.h"

void cmd_quit(server_t *server, client_t *client, char *args UNUSED)
{
    (void)server;
    send_response(client, CODE_QUIT, "Goodbye.");
    shutdown(client->socket, SHUT_RDWR);
}
