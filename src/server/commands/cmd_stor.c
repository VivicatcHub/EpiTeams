/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_stor.c
*/

#include "my_teams.h"
#include "commands.h"

static int validate_stor_args(client_t *client, char *args, char *path)
{
    if (!args || args[0] == '\0') {
        send_response(client, CODE_SYNTAX_ERROR_PARAM, "Filename required.");
        return 0;
    }
    if (client->mode == NONE) {
        send_response(client, CODE_CANNOT_OPEN_DATA,
            "Use PORT or PASV first.");
        return 0;
    }
    if (!build_path(client, args, path, PATH_MAX)) {
        send_response(client, CODE_FILE_UNAVAILABLE, "Invalid path.");
        return 0;
    }
    return 1;
}

static int transfer_stor_data(int data_fd, int file_fd)
{
    char buf[MAX_BUFFER];
    ssize_t n = read(data_fd, buf, sizeof(buf));

    while (n > 0) {
        write(file_fd, buf, n);
        n = read(data_fd, buf, sizeof(buf));
    }
    return 1;
}

static void child_stor(client_t *client, char const *path)
{
    int data_fd;
    int file_fd;
    int stor_success = 0;

    child_init(client);
    data_fd = open_data_conn(client);
    file_fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (data_fd >= 0 && file_fd >= 0)
        stor_success = transfer_stor_data(data_fd, file_fd);
    if (file_fd >= 0)
        close(file_fd);
    if (data_fd >= 0)
        close(data_fd);
    parent_cleanup_data(client);
    if (stor_success)
        send_response(client, CODE_TRANSFER_COMPLETE, "Transfer complete.");
    else
        send_response(client, CODE_ACTION_ABORTED, "Internal error.");
    _exit(0);
}

void cmd_stor(client_t *client, char *args)
{
    char path[PATH_MAX];
    pid_t pid;

    if (!validate_stor_args(client, args, path))
        return;
    send_response(client, CODE_FILE_STATUS_OK, "Ok to send data.");
    pid = fork();
    if (pid < 0) {
        send_response(client, CODE_ACTION_ABORTED, "Internal error.");
        parent_cleanup_data(client);
        return;
    }
    if (pid == 0)
        child_stor(client, path);
    client->child_process_pid = pid;
    parent_cleanup_data(client);
}
