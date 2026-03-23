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
    printf("USAGE: ./myftp port path\n");
    printf(" port is the port number on which the server socket listens\n");
    printf(" path is the path to the home directory for the Anonymous user\n");
}

static int validate_args(int argc, char **argv)
{
    int port;
    struct stat st;

    if (argc != 3) {
        fprintf(stderr, "Error: Invalid number of arguments\n");
        return -1;
    }
    port = atoi(argv[1]);
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Error: Invalid port number (must be 1-65535)\n");
        return -1;
    }
    if (stat(argv[2], &st) == -1 || !S_ISDIR(st.st_mode)) {
        fprintf(stderr, "Error: Invalid directory path\n");
        return -1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    if (argc == 2 && (strcmp(argv[1], "-h") == 0 ||
            strcmp(argv[1], "--help") == 0)) {
        display_usage();
        return 0;
    }
    if (validate_args(argc, argv) == -1) {
        display_usage();
        return 84;
    }
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, handle_signal);
    return start_server(argv[1], argv[2]);
}
