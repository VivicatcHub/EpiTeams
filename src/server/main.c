/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** main.c
*/

#include "my_teams.h"

static void display_usage(char const *prog_name)
{
    printf("USAGE: %s port\n", prog_name);
    printf("port is the port number on which the server socket listens.\n");
}

static int validate_args(char **argv)
{
    int port;

    port = atoi(argv[1]);
    if (port <= 0 || port > 65535) {
        printf("Error: Invalid port number (must be 1-65535)\n");
        return -1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Error: Invalid number of arguments\n");
        return EXIT_ERROR;
    }
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        display_usage(argv[0]);
        return 0;
    }
    if (validate_args(argv) == -1) {
        display_usage(argv[0]);
        return EXIT_ERROR;
    }
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, SIG_IGN);
    return start_server(argv[1]);
}
