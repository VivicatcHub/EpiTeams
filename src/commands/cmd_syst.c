/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_syst.c
*/

#include "my_teams.h"

void cmd_syst(client_t *client, char *args UNUSED)
{
    send_response(client, CODE_SYSTEM_TYPE, "UNIX Type: L8");
}
