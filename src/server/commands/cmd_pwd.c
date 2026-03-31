/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_pwd.c
*/

#include "my_teams.h"

void cmd_pwd(client_t *client, char *args UNUSED)
{
    char const *root = ((server_t *)client->server)->root_path;
    char const *rel;
    char msg[PATH_MAX + 4];

    if (!client->current_dir) {
        send_response(client, CODE_FILE_UNAVAILABLE,
            "No current directory.");
        return;
    }
    rel = client->current_dir + strlen(root);
    if (rel[0] == '\0')
        rel = "/";
    snprintf(msg, sizeof(msg), "\"%s\"", rel);
    send_response(client, CODE_PATHNAME_CREATED, msg);
}
