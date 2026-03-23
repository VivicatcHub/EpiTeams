/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_dele.c
*/

#include "my_teams.h"
#include "commands.h"

void cmd_dele(client_t *client, char *args)
{
    char path[PATH_MAX];
    struct stat st;

    if (!args || args[0] == '\0') {
        send_response(client, CODE_SYNTAX_ERROR_PARAM, "Filename required.");
        return;
    }
    if (!build_path(client, args, path, sizeof(path))) {
        send_response(client, CODE_FILE_UNAVAILABLE, "No such file.");
        return;
    }
    if (stat(path, &st) < 0 || S_ISDIR(st.st_mode)) {
        send_response(client, CODE_FILE_UNAVAILABLE, "File does not exist.");
        return;
    }
    if (unlink(path) < 0) {
        send_response(client, CODE_FILE_UNAVAILABLE, "Delete failed.");
        return;
    }
    send_response(client, CODE_FILE_ACTION_OK, "File deleted.");
}
