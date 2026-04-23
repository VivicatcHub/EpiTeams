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
void cmd_logout(server_t *server, client_t *client, char *args);
void cmd_users(server_t *server, client_t *client, char *args);
void cmd_user(server_t *server, client_t *client, char *args);
void cmd_send(server_t *server, client_t *client, char *args);
void cmd_messages(server_t *server, client_t *client, char *args);
void cmd_laugh(server_t *server, client_t *client, char *args);

// For debugging
void cmd_placeholder(server_t *server, client_t *client, char *args);

void cmd_create_team(server_t *server, client_t *client, char *args);
void cmd_create_channel(server_t *server, client_t *client, char *args);
void cmd_create_thread(server_t *server, client_t *client, char *args);
void cmd_create_comment(server_t *server, client_t *client, char *args);
void cmd_subscribe(server_t *server, client_t *client, char *args);
void cmd_unsubscribe(server_t *server, client_t *client, char *args);
void cmd_subscribed(server_t *server, client_t *client, char *args);
void cmd_info(server_t *server, client_t *client, char *args);
void cmd_list(server_t *server, client_t *client, char *args);
void cmd_info_thread(server_t *server, client_t *client, char *args);
void cmd_info_team(server_t *server, client_t *client, char *args);
void cmd_info_channel(server_t *server, client_t *client, char *args);
void cmd_list_channels(server_t *server, client_t *client, char *args);
void cmd_list_comments(server_t *server, client_t *client, char *args);
void cmd_list_teams(server_t *server, client_t *client, char *args);
void cmd_list_threads(server_t *server, client_t *client, char *args);
int count_connected_clients_by_username(server_t *server,
    char const *username);
int is_username_connected(server_t *server, char const *username);
void sync_user_connection_state(server_t *server, char const *username);
void notify_connected_clients(server_t *server, char const *username,
    char const *code, char const *msg);
// Helpers
user_t *find_user_by_name(server_t *server, char const *username);
user_t *find_user_by_uuid(server_t *server, uuid_t uuid);
team_t *find_team_by_uuid(server_t *server, uuid_t uuid);
channel_t *find_channel_by_uuid(server_t *server, uuid_t uuid);
thread_t *find_thread_by_uuid(server_t *server, uuid_t uuid);
team_t *find_team_by_name(server_t *server, char const *name);
channel_t *find_channel_by_name(server_t *server, char const *name);
thread_t *find_thread_by_title(server_t *server, char const *title);

int parse_and_verify_team(server_t *server, client_t *client,
    char *team_str, team_t **out_team);
int parse_and_get_channel(server_t *server, client_t *client,
    char *channel_str, channel_t **out_channel);
int parse_and_get_thread(server_t *server, client_t *client, char *thread_str,
    thread_t **out_thread);
typedef struct thread_context_params_s {
    char *thread_str;
    team_t *team;
    channel_t *channel;
    thread_t **out_thread;
} thread_context_params_t;

typedef struct channel_in_team_params_s {
    char *team_str;
    char *channel_str;
    team_t **out_team;
    channel_t **out_channel;
} channel_in_team_params_t;

typedef struct thread_in_channel_params_s {
    char *team_str;
    char *channel_str;
    char *thread_str;
    team_t **out_team;
    channel_t **out_channel;
    thread_t **out_thread;
} thread_in_channel_params_t;

int parse_and_get_thread_in_context(server_t *server, client_t *client,
    thread_context_params_t *params);
int is_user_logged_in(server_t *server, client_t *client);
int is_user_subscribed_to_team(user_t *user, uuid_t team_uuid);
int require_user_subscribed_to_team(server_t *server, client_t *client,
    user_t *user, team_t *team);
int parse_and_verify_channel_in_team(server_t *server, client_t *client,
    channel_in_team_params_t *params);
int parse_and_verify_thread_in_channel(server_t *server, client_t *client,
    thread_in_channel_params_t *params);

void dispatch_command(server_t *server, client_t *client, char *line);

typedef struct channel_create_params_s {
    char *team_str;
    char *name;
    char *desc;
} channel_create_params_t;

typedef struct thread_parse_params_s {
    char *team_str;
    char *channel_str;
    char *title;
    char *msg;
} thread_create_params_t;

typedef struct comment_parse_params_s {
    char *team_str;
    char *channel_str;
    char *thread_str;
    char *body;
} comment_create_params_t;

typedef struct team_create_exec_s {
    user_t *creator;
    char *name;
    char *desc;
} team_create_exec_t;

typedef struct thread_notify_context_s {
    uuid_t channel_uuid;
    user_t *author;
    client_t *client;
} thread_notify_context_t;

typedef struct thread_targets_s {
    channel_t *channel;
    user_t *author;
} thread_targets_t;

typedef struct comment_notify_context_s {
    thread_t *thread;
    user_t *author;
    client_t *client;
} comment_notify_context_t;

typedef struct comment_targets_s {
    thread_t *thread;
    user_t *author;
} comment_targets_t;

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

extern const command_t commands[];

#endif
