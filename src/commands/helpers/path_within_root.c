/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** path_without_root.c
*/

#include "my_teams.h"

int path_within_root(client_t *client, char const *resolved)
{
    char const *root = ((server_t *)client->server)->root_path;
    size_t rlen = strlen(root);

    if (strncmp(resolved, root, rlen) != 0)
        return 0;
    if (resolved[rlen] != '\0' && resolved[rlen] != '/')
        return 0;
    return 1;
}
