/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_quit.c
*/

#include "my_teams.h"

void cmd_quit(server_t *server UNUSED, client_t *client, char *args UNUSED)
{
    send_response(client, CODE_QUIT, "Goodbye.");
    shutdown(client->socket, SHUT_RDWR);
}
