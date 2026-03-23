/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_retr.c
*/

#include "my_teams.h"
#include "commands.h"

static int validate_retr_args(client_t *client, char *args, char *path)
{
    struct stat st;

    if (!args || args[0] == '\0') {
        send_response(client, CODE_SYNTAX_ERROR_PARAM, "Filename required.");
        return 0;
    }
    if (client->mode == NONE) {
        send_response(client, CODE_CANNOT_OPEN_DATA, "Use PORT / PASV first.");
        return 0;
    }
    if (!build_path(client, args, path, PATH_MAX)
        || stat(path, &st) < 0 || S_ISDIR(st.st_mode)) {
        send_response(client, CODE_FILE_UNAVAILABLE, "File unavailable.");
        return 0;
    }
    return 1;
}

static int transfer_file(int file_fd, int data_fd)
{
    char buf[MAX_BUFFER];
    ssize_t n;

    n = read(file_fd, buf, sizeof(buf));
    while (n > 0) {
        if (write(data_fd, buf, n) != n)
            return 0;
        n = read(file_fd, buf, sizeof(buf));
    }
    return n == 0;
}

static void send_result(client_t *client, int file_fd,
    int data_fd, int retr_success)
{
    if (retr_success)
        send_response(client, CODE_TRANSFER_COMPLETE, "Transfer complete.");
    if (file_fd < 0)
        send_response(client, CODE_FILE_UNAVAILABLE, "File unavailable.");
    if (data_fd < 0)
        send_response(client, CODE_CANNOT_OPEN_DATA,
            "Can't open data connection.");
    if (!retr_success && file_fd >= 0 && data_fd >= 0)
        send_response(client, CODE_CONNECTION_CLOSED,
            "Connection closed; transfer aborted.");
}

static void child_retr(client_t *client, char const *path)
{
    int file_fd;
    int data_fd;
    int retr_success = 0;

    child_init(client);
    file_fd = open(path, O_RDONLY);
    data_fd = open_data_conn(client);
    if (file_fd >= 0 && data_fd >= 0)
        retr_success = transfer_file(file_fd, data_fd);
    if (file_fd >= 0)
        close(file_fd);
    if (data_fd >= 0)
        close(data_fd);
    parent_cleanup_data(client);
    send_result(client, file_fd, data_fd, retr_success);
    _exit(0);
}

void cmd_retr(client_t *client, char *args)
{
    char path[PATH_MAX];
    pid_t pid;

    if (!validate_retr_args(client, args, path))
        return;
    send_response(client, CODE_FILE_STATUS_OK,
        "Opening data connection for file transfer.");
    pid = fork();
    if (pid < 0) {
        send_response(client, CODE_ACTION_ABORTED, "Internal error.");
        parent_cleanup_data(client);
        return;
    }
    if (pid == 0)
        child_retr(client, path);
    client->child_process_pid = pid;
    parent_cleanup_data(client);
}
