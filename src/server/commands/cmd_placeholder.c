/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_placeholder.c
*/

#include "my_teams.h"

void cmd_placeholder(server_t *server UNUSED, client_t *client,
    char *args UNUSED)
{
    char buf[MAX_BUFFER];

    snprintf(buf, sizeof(buf), "If you see this, it means you somehow"
        " reached a placeholder :p");
    send_response(client, CODE_COMMAND_OK, buf);
}
