/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_list.c
*/

#include "my_teams.h"
#include "commands.h"

static int validate_path(client_t *client, char *args, char *path)
{
    if (args && args[0] != '\0') {
        if (!build_path(client, args, path, PATH_MAX)
            || stat(path, &(struct stat){0}) < 0) {
            send_response(client, CODE_FILE_UNAVAILABLE,
                "No such file or directory.");
            return 0;
        }
    } else {
        strncpy(path, client->current_dir, PATH_MAX - 1);
        path[PATH_MAX - 1] = '\0';
    }
    return 1;
}

static void child_list(client_t *client, char const *path)
{
    int data_fd;

    child_init(client);
    data_fd = open_data_conn(client);
    if (data_fd >= 0) {
        list_command(data_fd, path);
        close(data_fd);
    }
    parent_cleanup_data(client);
    send_response(client, CODE_TRANSFER_COMPLETE, "Directory send OK.");
    _exit(0);
}

void cmd_list(client_t *client, char *args)
{
    char path[PATH_MAX];
    pid_t pid;

    if (client->mode == NONE) {
        send_response(client, CODE_CANNOT_OPEN_DATA, "Use PORT / PASV first.");
        return;
    }
    if (!validate_path(client, args, path))
        return;
    send_response(client, CODE_FILE_STATUS_OK, "Sending directory listing.");
    pid = fork();
    if (pid < 0) {
        send_response(client, CODE_ACTION_ABORTED, "Internal error.");
        parent_cleanup_data(client);
        return;
    }
    if (pid == 0)
        child_list(client, path);
    client->child_process_pid = pid;
    parent_cleanup_data(client);
}
