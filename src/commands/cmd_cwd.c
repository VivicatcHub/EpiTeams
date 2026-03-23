/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_cwd.c
*/

#include "my_teams.h"
#include "commands.h"

void cmd_cwd(client_t *client, char *args)
{
    char path[PATH_MAX];
    struct stat st;

    if (!args || args[0] == '\0') {
        send_response(client, CODE_SYNTAX_ERROR_PARAM, "Path required.");
        return;
    }
    if (!build_path(client, args, path, sizeof(path))) {
        send_response(client, CODE_FILE_UNAVAILABLE,
            "No such file or directory.");
        return;
    }
    if (stat(path, &st) < 0 || !S_ISDIR(st.st_mode)) {
        send_response(client, CODE_FILE_UNAVAILABLE, "No such directory.");
        return;
    }
    free(client->current_dir);
    client->current_dir = strdup(path);
    send_response(client, CODE_FILE_ACTION_OK,
        "Directory successfully changed.");
}
