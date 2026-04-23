/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** user_presence.c
*/

#include "my_teams.h"

int count_connected_clients_by_username(server_t *server,
    char const *username)
{
    int count = 0;

    if (!server || !username)
        return 0;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!server->clients[i] || !server->clients[i]->username)
            continue;
        if (strcmp(server->clients[i]->username, username) == 0)
            count++;
    }
    return count;
}

int is_username_connected(server_t *server, char const *username)
{
    return count_connected_clients_by_username(server, username) > 0;
}

static void update_user_presence(user_t *user, int connected_count)
{
    char uuid_buf[UUID_STR_LEN];

    if (connected_count > 0 && user->status != ONLINE) {
        user->status = ONLINE;
        uuid_to_string(user->uuid, uuid_buf);
        server_event_user_logged_in(uuid_buf);
        return;
    }
    if (connected_count == 0 && user->status != OFFLINE) {
        user->status = OFFLINE;
        uuid_to_string(user->uuid, uuid_buf);
        server_event_user_logged_out(uuid_buf);
    }
}

void sync_user_connection_state(server_t *server, char const *username)
{
    user_t *user = NULL;
    int connected_count;

    if (!server || !username)
        return;
    user = find_user_by_name(server, username);
    if (!user)
        return;
    connected_count = count_connected_clients_by_username(server, username);
    update_user_presence(user, connected_count);
}

void notify_connected_clients(server_t *server, char const *username,
    char const *code, char const *msg)
{
    if (!server || !username)
        return;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!server->clients[i] || !server->clients[i]->username)
            continue;
        if (strcmp(server->clients[i]->username, username) != 0)
            continue;
        send_response(server->clients[i], code, msg);
    }
}
