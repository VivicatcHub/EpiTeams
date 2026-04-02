/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_pasv.c
*/

#include "my_teams.h"

static int create_data_socket(client_t *client)
{
    int sock;
    int opt = 1;
    struct sockaddr_in bound;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        send_response(client, CODE_CANNOT_OPEN_DATA, "Can't create socket.");
        return -1;
    }
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    memset(&bound, 0, sizeof(bound));
    bound.sin_family = AF_INET;
    bound.sin_addr.s_addr = INADDR_ANY;
    if (bind(sock, (struct sockaddr *)&bound, sizeof(bound)) < 0
        || listen(sock, 1) < 0) {
        close(sock);
        send_response(client, CODE_CANNOT_OPEN_DATA, "Data socket mistake.");
        return -1;
    }
    return sock;
}

static int get_bound_port(client_t *client, int sock,
    struct sockaddr_in *bound)
{
    socklen_t len = sizeof(*bound);

    if (getsockname(sock, (struct sockaddr *)bound, &len) < 0) {
        close(sock);
        send_response(client, CODE_CANNOT_OPEN_DATA, "Can't get port.");
        return -1;
    }
    return 0;
}

static void send_pasv_response(client_t *client, int sock,
    struct sockaddr_in *bound)
{
    struct sockaddr_in local;
    socklen_t len = sizeof(local);
    unsigned char *ip;
    int p1;
    int p2;
    char msg[64];

    getsockname(client->socket, (struct sockaddr *)&local, &len);
    ip = (unsigned char *)&local.sin_addr.s_addr;
    p1 = get_port(&bound->sin_port) >> 8;
    p2 = get_port(&bound->sin_port) & 0xFF;
    client->data_socket = sock;
    client->mode = PASSIVE;
    snprintf(msg, sizeof(msg),
        "Entering Passive Mode (%d,%d,%d,%d,%d,%d)",
        ip[0], ip[1], ip[2], ip[3], p1, p2);
    send_response(client, CODE_PASSIVE_MODE, msg);
}

void cmd_pasv(server_t *server UNUSED, client_t *client, char *args UNUSED)
{
    struct sockaddr_in bound;
    int sock;

    if (client->data_socket != -1) {
        close(client->data_socket);
        client->data_socket = -1;
    }
    sock = create_data_socket(client);
    if (sock < 0)
        return;
    if (get_bound_port(client, sock, &bound) < 0)
        return;
    send_pasv_response(client, sock, &bound);
}
