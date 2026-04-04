/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** server_start.c
*/

#include "my_teams.h"
#include "serialization.h"

static void init_server_data(server_t *server)
{
    TAILQ_INIT(&server->data.users);
    TAILQ_INIT(&server->data.teams);
    TAILQ_INIT(&server->data.channels);
    TAILQ_INIT(&server->data.threads);
    TAILQ_INIT(&server->data.comments);
    TAILQ_INIT(&server->data.messages);
}

static int setup_server(server_t *server, char *port_str)
{
    server->port = atoi(port_str);
    server->socket = -1;
    server->state = RUNNING;
    for (int i = 0; i < MAX_CLIENTS; i++)
        server->clients[i] = NULL;
    init_server_data(server);
    return 0;
}

static int load_and_init(server_t *server)
{
    if (load_server_data(&server->data) == -1) {
        printf("Error: Failed to load server data\n");
        return EXIT_ERROR;
    }
    printf("Server data loaded successfully\n");
    printf("Starting \"myteams\" server on port %d\n", server->port);
    if (init_server(server) == -1) {
        printf("Error: Failed to create server\n");
        cleanup_server_data(&server->data);
        return EXIT_ERROR;
    }
    return 0;
}

static void save_and_clean(server_t *server)
{
    printf("Saving server data...\n");
    save_server_data(&server->data);
    cleanup_server(server);
}

int start_server(char *port_str)
{
    server_t server;

    if (setup_server(&server, port_str) == -1)
        return EXIT_ERROR;
    if (load_and_init(&server) == -1)
        return EXIT_ERROR;
    run_server(&server);
    save_and_clean(&server);
    return 0;
}
