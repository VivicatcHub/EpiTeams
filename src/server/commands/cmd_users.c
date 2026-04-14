/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_users.c
*/

#include "my_teams.h"

static void collect_connected_usernames(server_t *server,
    char **out, int *count)
{
    *count = 0;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (server->clients[i] == NULL)
            continue;
        if (server->clients[i]->username == NULL)
            continue;
        out[*count] = server->clients[i]->username;
        (*count)++;
    }
}

static int is_name_in_list(char const *name, char **list, int count)
{
    for (int i = 0; i < count; i++) {
        if (strcmp(name, list[i]) == 0)
            return 1;
    }
    return 0;
}

static void send_user_status(client_t *client, user_t *user, int online)
{
    char buf[MAX_BUFFER];
    char uuid_buf[UUID_STR_LEN];

    uuid_to_string(user->uuid, uuid_buf);
    snprintf(buf, MAX_BUFFER, "%s (%s) -> %s",
        user->name, uuid_buf, online ? "ONLINE" : "OFFLINE");
    send_response(client, CODE_COMMAND_OK, buf);
}

void cmd_users(server_t *server, client_t *client UNUSED, char *args UNUSED)
{
    char *connected_usernames[MAX_CLIENTS];
    int connected_count = 0;
    int online = 0;
    user_t *user;

    collect_connected_usernames(server, connected_usernames, &connected_count);
    TAILQ_FOREACH(user, &server->data.users, entries) {
        online = is_name_in_list(user->name, connected_usernames,
            connected_count);
        send_user_status(client, user, online);
    }
    send_response(client, CODE_COMMAND_END, "End of users list");
}
