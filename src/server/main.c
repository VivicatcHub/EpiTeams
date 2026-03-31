/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** main.c
*/

#include "my_teams.h"

void handle_signal(int signum UNUSED)
{
    printf("\nShutting down server...\n");
    exit(0);
}

static void display_usage(void)
{
    printf("USAGE: ./myteams_server port\n");
    printf("port is the port number on which the server socket listens.\n");
}

static int validate_args(char **argv)
{
    int port;

    port = atoi(argv[1]);
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Error: Invalid port number (must be 1-65535)\n");
        return -1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Error: Invalid number of arguments\n");
        return EXIT_ERROR;
    }
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        display_usage();
        return 0;
    }
    if (validate_args(argv) == -1) {
        display_usage();
        return EXIT_ERROR;
    }
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, handle_signal);
    return start_server(argv[1]);
}
