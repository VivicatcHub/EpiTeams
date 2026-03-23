/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_cdup.c
*/

#include "my_teams.h"
#include "commands.h"

void cmd_cdup(client_t *client, char *args UNUSED)
{
    cmd_cwd(client, "..");
}
