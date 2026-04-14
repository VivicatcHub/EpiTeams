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

void dispatch_command(server_t *server, client_t *client, char *line)
{
    char name[16];
    char *args;

    parse_command(line, name, &args);
    for (int i = 0; commands[i].name != NULL; i++) {
        if (strcasecmp(commands[i].name, name) != 0)
            continue;
        if (!no_auth_required(name) && client->state != AUTHENTICATED) {
            send_response(client, CODE_NOT_LOGGED_IN, "Not logged in.");
            return;
        }
        commands[i].handler(server, client, args);
        return;
    }
    send_response(client, CODE_SYNTAX_ERROR, "Unknown command.");
}
