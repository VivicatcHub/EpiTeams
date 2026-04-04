/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** commands.h
*/

#ifndef COMMANDS_H
    #define COMMANDS_H

    #include "client.h"

typedef void (*cmd_handler_t)(server_t *, client_t *, char *);

typedef struct command_s {
    char *name;
    cmd_handler_t handler;
} command_t;

void cmd_login(server_t *server, client_t *client, char *args);
void cmd_users(server_t *server, client_t *client, char *args);
void cmd_user(server_t *server, client_t *client, char *args);
void cmd_send(server_t *server, client_t *client, char *args);
void cmd_messages(server_t *server, client_t *client, char *args);
void cmd_quit(server_t *server, client_t *client, char *args);

// For debugging
void cmd_placeholder(server_t *server, client_t *client, char *args);

// Helpers
user_t *find_user_by_name(server_t *server, char const *username);
user_t *find_user_by_uuid(server_t *server, uuid_t uuid);

void dispatch_command(server_t *server, client_t *client, char *line);

extern const command_t commands[];
#endif
