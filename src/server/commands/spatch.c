/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** spatch.c
*/

#include "my_teams.h"

const command_t commands[] = {
    {"USERS", cmd_users},
    {"LOGIN", cmd_login},
    {"LOGOUT", cmd_logout},
    {"SEND", cmd_send},
    {"MESSAGES", cmd_messages},
    {"USER", cmd_user},
    {"CREATE_TEAM", cmd_create_team},
    {"CREATE_CHANNEL", cmd_create_channel},
    {"CREATE_THREAD", cmd_create_thread},
    {"CREATE_COMMENT", cmd_create_comment},
    {"SUBSCRIBE", cmd_subscribe},
    {"UNSUBSCRIBE", cmd_unsubscribe},
    {"SUBSCRIBED", cmd_subscribed},
    {"LAUGH", cmd_laugh},
    {"LIST_TEAMS", cmd_list_teams},
    {"LIST_CHANNELS", cmd_list_channels},
    {"LIST_THREADS", cmd_list_threads},
    {"LIST_COMMENTS", cmd_list_comments},
    {"INFO_USER", cmd_info},
    {"INFO_TEAM", cmd_info_team},
    {"INFO_CHANNEL", cmd_info_channel},
    {"INFO_THREAD", cmd_info_thread},
    {NULL, NULL}
};

static int no_auth_required(char const *name)
{
    return strcasecmp(name, "LOGIN") == 0
        || strcasecmp(name, "QUIT") == 0
        || strcasecmp(name, "HELP") == 0
        || strcasecmp(name, "LAUGH") == 0;
}

static void parse_command(char *line, char *name, char **args)
{
    char *space;

    snprintf(name, 16, "%s", line);
    space = strchr(name, ' ');
    if (space) {
        *space = '\0';
        *args = strchr(line, ' ') + 1;
    } else {
        *args = NULL;
    }
}

static int handle_ack(client_t *client, char const *name)
{
    if (strcasecmp(name, "ACK") != 0)
        return 0;
    client->waiting_for_client_receipt = 0;
    return 1;
}

static int enqueue_pending_command(client_t *client, char const *line)
{
    pending_command_t *pending = malloc(sizeof(*pending));

    if (!pending)
        return -1;
    pending->line = strdup(line);
    if (!pending->line) {
        free(pending);
        return -1;
    }
    pending->next = NULL;
    if (!client->pending_commands_tail) {
        client->pending_commands_head = pending;
        client->pending_commands_tail = pending;
        return 0;
    }
    client->pending_commands_tail->next = pending;
    client->pending_commands_tail = pending;
    return 0;
}

static void execute_deferred_commands(server_t *server, client_t *client)
{
    pending_command_t *pending;

    while (!client->waiting_for_client_receipt &&
        client->pending_commands_head) {
        pending = client->pending_commands_head;
        client->pending_commands_head = pending->next;
        if (!client->pending_commands_head)
            client->pending_commands_tail = NULL;
        dispatch_command(server, client, pending->line);
        free(pending->line);
        free(pending);
    }
}

static void dispatch_for_loop(server_t *server, client_t *client, char *name,
    char *args)
{
    for (int i = 0; commands[i].name != NULL; i++) {
        if (strcasecmp(commands[i].name, name) != 0) {
            continue;
        }
        if (!no_auth_required(name) && client->state != AUTHENTICATED) {
            send_response(client, CODE_NOT_LOGGED_IN, "Not logged in.");
            return;
        }
        commands[i].handler(server, client, args);
        return;
    }
}

void dispatch_command(server_t *server, client_t *client, char *line)
{
    char name[16];
    char *args;

    parse_command(line, name, &args);
    if (handle_ack(client, name)) {
        execute_deferred_commands(server, client);
        return;
    }
    if (client->waiting_for_client_receipt) {
        if (enqueue_pending_command(client, line) == -1)
            send_response(client, CODE_ACTION_ABORTED,
                "Internal error while queueing command.");
        return;
    }
    printf("Command: '%s' handle\n", name);
    dispatch_for_loop(server, client, name, args);
    send_response(client, CODE_SYNTAX_ERROR, "Unknown command.");
}
