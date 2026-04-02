/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_noop.c
*/

#include "my_teams.h"

void cmd_noop(server_t *server UNUSED, client_t *client, char *args UNUSED)
{
    if (args && args[0] != '\0') {
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "NOOP command does not take any parameters.");
        return;
    }
    send_response(client, CODE_COMMAND_OK, "OK.");
}
