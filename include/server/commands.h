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
void cmd_pasv(server_t *server, client_t *client, char *args);
void cmd_port(server_t *server, client_t *client, char *args);
void cmd_laugh(server_t *server, client_t *client, char *args);

// For debugging
void cmd_placeholder(server_t *server, client_t *client, char *args);

void cmd_create_team(server_t *server, client_t *client, char *args);
void cmd_create_channel(server_t *server, client_t *client, char *args);
void cmd_create_thread(server_t *server, client_t *client, char *args);
void cmd_create_comment(server_t *server, client_t *client, char *args);
void cmd_use(server_t *server, client_t *client, char *args);

// Helpers
user_t *find_user_by_name(server_t *server, char const *username);
user_t *find_user_by_uuid(server_t *server, uuid_t uuid);
team_t *find_team_by_uuid(server_t *server, uuid_t uuid);
channel_t *find_channel_by_uuid(server_t *server, uuid_t uuid);
thread_t *find_thread_by_uuid(server_t *server, uuid_t uuid);
team_t *find_team_by_name(server_t *server, char const *name);
channel_t *find_channel_by_name(server_t *server, char const *name);
thread_t *find_thread_by_title(server_t *server, char const *title);

void dispatch_command(server_t *server, client_t *client, char *line);

typedef struct channel_create_params_s {
    char *team_str;
    char *name;
    char *desc;
} channel_create_params_t;

typedef struct thread_parse_params_s {
    char *channel_str;
    char *title;
    char *msg;
} thread_create_params_t;

typedef struct thread_init_s {
    uuid_t channel_uuid;
    uuid_t team_uuid;
    uuid_t author_uuid;
    char *title;
    char *msg;
} thread_init_t;

typedef struct comment_init_s {
    uuid_t thread_uuid;
    uuid_t author_uuid;
    char *body;
} comment_init_t;

typedef struct use_context_s {
    server_t *server;
    client_t *client;
    team_t *team;
} use_context_t;

extern const command_t commands[];
#endif
