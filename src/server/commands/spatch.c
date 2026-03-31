/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** spatch.c
*/

#include "my_teams.h"

const command_t commands[] = {
    {"USER", cmd_user},
    {"PASS", cmd_pass},
    {"CWD", cmd_cwd},
    {"CDUP", cmd_cdup},
    {"QUIT", cmd_quit},
    {"DELE", cmd_dele},
    {"PWD", cmd_pwd},
    {"PASV", cmd_pasv},
    {"PORT", cmd_port},
    {"HELP", cmd_help},
    {"NOOP", cmd_noop},
    {"RETR", cmd_retr},
    {"STOR", cmd_stor},
    {"LIST", cmd_list},
    {"SYST", cmd_syst},
    {"FEAT", cmd_feat},
    {"SHUTDOWN", cmd_shutdown},
    {NULL, NULL}
};

static int no_auth_required(char const *name)
{
    return strcasecmp(name, "USER") == 0
        || strcasecmp(name, "PASS") == 0
        || strcasecmp(name, "QUIT") == 0
        || strcasecmp(name, "HELP") == 0;
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

void dispatch_command(client_t *client, char *line)
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
        commands[i].handler(client, args);
        return;
    }
    send_response(client, CODE_SYNTAX_ERROR, "Unknown command.");
}
