/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_help.c
*/

#include "my_teams.h"

const help_entries_t help_entries[] = {
    {"USER", "USER <SP> <username> <CRLF>\r\n\t"
        "Specify user for authentication."},
    {"PASS", "PASS <SP> <password> <CRLF>\r\n\t"
        "Specify password for authentication."},
    {"QUIT", "QUIT <CRLF>\r\n\tClose the control "
        "connection and log out."},
    {"HELP", "HELP [<SP> <string>] <CRLF>\r\n\tList available commands"
        ", or show help for a specific command."},
    {"NOOP", "NOOP <CRLF>\r\n\tDo nothing. "
        "The server sends a 200 reply."},
    {"PORT", "PORT <SP> <h1,h2,h3,h4,p1,p2> <CRLF>\r\n\tEnable active mode:"
        " specify the client address and port for data transfer."},
    {"PASV", "PASV <CRLF>\r\n\tEnable passive mode: "
        "server opens a data port and returns its address."},
    {NULL, NULL}
};

void cmd_help(client_t *client, char *args)
{
    if (!args || args[0] == '\0') {
        send_response(client, CODE_HELP_MESSAGE,
            "Commands supported:\r\n"
            "\tUSER\tPASS\tQUIT\tHELP\tNOOP\r\n"
            "\tPORT\tPASV");
        return;
    }
    for (int i = 0; help_entries[i].name != NULL; i++) {
        if (strcasecmp(args, help_entries[i].name) == 0) {
            send_response(client, CODE_HELP_MESSAGE,
                help_entries[i].description);
            return;
        }
    }
    send_response(client, CODE_SYNTAX_ERROR_PARAM,
        "Unknown command.");
}
