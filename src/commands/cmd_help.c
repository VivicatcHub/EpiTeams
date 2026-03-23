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
    {"CWD", "CWD <SP> <pathname> <CRLF>\r\n\t"
        "Change working directory to the specified path."},
    {"CDUP", "CDUP <CRLF>\r\n\tChange working directory "
        "to the parent directory."},
    {"PWD", "PWD <CRLF>\r\n\tPrint the current working directory."},
    {"DELE", "DELE <SP> <pathname> <CRLF>\r\n\t"
        "Delete the specified file on the server." },
    {"PORT", "PORT <SP> <h1,h2,h3,h4,p1,p2> <CRLF>\r\n\tEnable active mode:"
        " specify the client address and port for data transfer."},
    {"PASV", "PASV <CRLF>\r\n\tEnable passive mode: "
        "server opens a data port and returns its address."},
    {"LIST", "LIST [<SP> <pathname>] <CRLF>\r\n\t"
        "List files in the current or specified directory."},
    {"RETR", "RETR <SP> <pathname> <CRLF>\r\n\t"
        "Download the specified file from the server."},
    {"STOR", "STOR <SP> <pathname> <CRLF>\r\n\t"
        "Upload a file to the specified path on the server."},
    {NULL, NULL}
};

void cmd_help(client_t *client, char *args)
{
    if (!args || args[0] == '\0') {
        send_response(client, CODE_HELP_MESSAGE,
            "Commands supported:\r\n"
            "\tUSER\tPASS\tQUIT\tHELP\tNOOP\r\n"
            "\tCWD\tCDUP\tPWD\tDELE\r\n"
            "\tPORT\tPASV\tLIST\tRETR\tSTOR");
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
