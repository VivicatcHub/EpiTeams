/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_laugh.c
*/

#include "my_teams.h"

void cmd_laugh(server_t *server UNUSED, client_t *client, char *args UNUSED)
{
    send_response(client, CODE_COMMAND_OK, "haha");
}
