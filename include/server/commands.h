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
void cmd_quit(server_t *server, client_t *client, char *args);
void cmd_pasv(server_t *server, client_t *client, char *args);
void cmd_port(server_t *server, client_t *client, char *args);
void cmd_laugh(server_t *server, client_t *client, char *args);

// For debugging
void cmd_placeholder(server_t *server, client_t *client, char *args);

void dispatch_command(server_t *server, client_t *client, char *line);

extern const command_t commands[];

#endif
