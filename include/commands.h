/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** commands.h
*/

#ifndef COMMANDS_H
    #define COMMANDS_H

    #include "client.h"

typedef void (*cmd_handler_t)(client_t *, char *);

typedef struct command_s {
    char *name;
    cmd_handler_t handler;
} command_t;

void cmd_user(client_t *client, char *args);
void cmd_pass(client_t *client, char *args);
void cmd_cwd(client_t *client, char *args);
void cmd_cdup(client_t *client, char *args);
void cmd_quit(client_t *client, char *args);
void cmd_dele(client_t *client, char *args);
void cmd_pwd(client_t *client, char *args);
void cmd_pasv(client_t *client, char *args);
void cmd_port(client_t *client, char *args);
void cmd_help(client_t *client, char *args);
void cmd_noop(client_t *client, char *args);
void cmd_retr(client_t *client, char *args);
void cmd_stor(client_t *client, char *args);
void cmd_list(client_t *client, char *args);
void cmd_syst(client_t *client, char *args);
void cmd_feat(client_t *client, char *args);
void cmd_shutdown(client_t *client, char *args);

void dispatch_command(client_t *client, char *line);

int open_data_conn(client_t *client);
void parent_cleanup_data(client_t *client);
int build_path(client_t *client, char const *args, char *out, size_t sz);
int path_within_root(client_t *client, char const *resolved);
void list_command(int data_fd, char const *path);
void child_init(client_t *client);

extern const command_t commands[];
#endif
