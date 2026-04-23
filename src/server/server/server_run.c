/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** server_run.c
*/

#include "my_teams.h"

static void build_stdin_pollfd(struct pollfd *fds, client_t **client_map,
    int *nfds)
{
    fds[*nfds].fd = STDIN_FILENO;
    fds[*nfds].events = POLLIN;
    fds[*nfds].revents = 0;
    client_map[*nfds] = NULL;
    (*nfds)++;
}

static int build_pollfds(server_t *server,
    struct pollfd *fds, client_t **client_map)
{
    int nfds = 0;

    fds[nfds].fd = server->socket;
    fds[nfds].events = POLLIN;
    fds[nfds].revents = 0;
    client_map[nfds] = NULL;
    nfds++;
    build_stdin_pollfd(fds, client_map, &nfds);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (server->clients[i] != NULL) {
            fds[nfds].fd = server->clients[i]->socket;
            fds[nfds].events = POLLIN;
            fds[nfds].revents = 0;
            client_map[nfds] = server->clients[i];
            nfds++;
        }
    }
    return nfds;
}

static int do_poll(struct pollfd *fds, int nfds)
{
    if (poll(fds, nfds, -1) != -1)
        return 0;
    if (errno == EINTR)
        return EXIT_FAILURE;
    perror("poll");
    return -1;
}

static void handle_client_event(server_t *server,
    struct pollfd *fds, client_t **client_map, int i)
{
    if (client_map[i] == NULL)
        return;
    if (fds[i].revents & POLLIN)
        handle_client_data(server, client_map[i]);
    else if (fds[i].revents & (POLLHUP | POLLERR))
        remove_client(server, client_map[i]);
}

static void handle_events(server_t *server,
    struct pollfd *fds, client_t **client_map, int nfds)
{
    if (fds[0].revents & POLLIN)
        accept_client(server);
    if (fds[1].revents & POLLIN)
        handle_server_events(server);
    for (int i = 2; i < nfds; i++)
        handle_client_event(server, fds, client_map, i);
}

static void logout_all_clients(server_t *server)
{
    client_t *client;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        client = server->clients[i];
        if (client == NULL)
            continue;
        if (client->state != OFFLINE) {
            if (client->username) {
                logout_and_respond(server, client);
                return;
            }
            client->state = CONNECTED;
            send_response(client, CODE_LOGOUT_SUCCESS, "Logged out.");
        }
    }
}

void run_server(server_t *server)
{
    struct pollfd fds[MAX_CLIENTS + 2];
    client_t *client_map[MAX_CLIENTS + 2];
    int nfds;
    int ret;

    while (server->state == RUNNING) {
        nfds = build_pollfds(server, fds, client_map);
        ret = do_poll(fds, nfds);
        if (ret == -1)
            break;
        if (ret == EXIT_FAILURE) {
            server->state = STOPPING;
            break;
        }
        if (ret == 0)
            handle_events(server, fds, client_map, nfds);
    }
    logout_all_clients(server);
}
