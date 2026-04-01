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
void cmd_quit(client_t *client, char *args);
void cmd_pasv(client_t *client, char *args);
void cmd_port(client_t *client, char *args);
void cmd_help(client_t *client, char *args);
void cmd_noop(client_t *client, char *args);

void cmd_placeholder(client_t *client, char *args); // For debugging

void dispatch_command(client_t *client, char *line);

extern const command_t commands[];
#endif
