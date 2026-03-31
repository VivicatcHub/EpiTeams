/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_feat.c
*/

#include "my_teams.h"

void cmd_feat(client_t *client, char *args UNUSED)
{
    send_response(client, CODE_SYSTEM_STATUS, "No-Features");
}
