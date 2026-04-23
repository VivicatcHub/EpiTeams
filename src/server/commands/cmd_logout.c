/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_logout.c
*/

#include "my_teams.h"

void logout_and_respond(server_t *server, client_t *client)
{
    char uuid_buf[UUID_STR_LEN];
    char buf[MAX_BUFFER];
    user_t *user = NULL;

    user = find_user_by_name(server, client->username);
    if (user) {
        uuid_to_string(user->uuid, uuid_buf);
    } else {
        uuid_buf[0] = '\0';
    }
    sync_user_connection_state(server, client->username);
    snprintf(buf, MAX_BUFFER, "Logged out %s (%s).",
        client->username, uuid_buf);
    free(client->username);
    client->username = NULL;
    client->state = CONNECTED;
    send_response(client, CODE_LOGOUT_SUCCESS, buf);
}

void cmd_logout(server_t *server UNUSED, client_t *client, char *args UNUSED)
{
    if (client->state != AUTHENTICATED) {
        send_response(client, CODE_NOT_LOGGED_IN, "Not logged in.");
        return;
    }
    if (client->username) {
        logout_and_respond(server, client);
        return;
    }
    client->state = CONNECTED;
    send_response(client, CODE_LOGOUT_SUCCESS, "Logged out.");
}
