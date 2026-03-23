/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** server_events.c
*/

#include "my_teams.h"

void handle_server_events(server_t *server)
{
    char *line = NULL;
    size_t len = 0;

    if (getline(&line, &len, stdin) == -1 ||
        strcasecmp(line, "QUIT\n") == 0 ||
        strcasecmp(line, "EXIT\n") == 0 ||
        strcasecmp(line, "SHUTDOWN\n") == 0) {
        server->state = STOPPING;
    }
    free(line);
}
