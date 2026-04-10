/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_login.c
*/

#include "my_teams.h"


static int check_arg(client_t *client, char *arg)
{
    if (strlen(arg) > MAX_NAME_LENGTH) {
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "Username too long.");
        return -1;
    }
    if (strlen(arg) == 0) {
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "Username cannot be empty.");
        return -1;
    }
    for (size_t i = 0; arg[i] != '\0'; i++) {
        if (arg[i] == ' ') {
            send_response(client, CODE_SYNTAX_ERROR_PARAM,
                "Username cannot contain spaces.");
            return -1;
        }
    }
    return 0;
}

static void login_existing_user(client_t *client, char *username)
{
    client->username = strdup(username);
    client->state = AUTHENTICATED;
}

static void create_user_and_persist(server_t *server, client_t *client,
    char *username)
{
    user_t *new_user = malloc(sizeof(user_t));
    char uuid_buf[UUID_STR_LEN];
    char buf[MAX_BUFFER];

    if (!new_user)
        return;
    uuid_generate(new_user->uuid);
    uuid_to_string(new_user->uuid, uuid_buf);
    strncpy(new_user->name, username, MAX_NAME_LENGTH - 1);
    new_user->name[MAX_NAME_LENGTH - 1] = '\0';
    new_user->status = ONLINE;
    TAILQ_INIT(&new_user->subscribed_teams);
    TAILQ_INSERT_TAIL(&server->data.users, new_user, entries);
    server_event_user_created(uuid_buf, username);
    client->username = strdup(username);
    client->state = AUTHENTICATED;
    snprintf(buf, MAX_BUFFER, "Logged in as %s (%s).", username, uuid_buf);
    send_response(client, CODE_LOGIN_SUCCESS, buf);
}

static int is_name_connected(server_t *server, char const *name)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!server->clients[i])
            continue;
        if (!server->clients[i]->username)
            continue;
        if (strcmp(server->clients[i]->username, name) == 0)
            return 1;
    }
    return 0;
}

static int handle_user(client_t *client, server_t *server, char *username)
{
    user_t *user = find_user_by_name(server, username);
    char uuid_buf[UUID_STR_LEN];
    char buf[MAX_BUFFER];

    if (user) {
        if (is_name_connected(server, username)) {
            send_response(client, CODE_USER_ALREADY_CONNECTED,
                "User already connected.");
            return -1;
        }
        login_existing_user(client, username);
        uuid_to_string(user->uuid, uuid_buf);
        server_event_user_logged_in(uuid_buf);
        snprintf(buf, MAX_BUFFER, "Logged in as %s (%s).", username, uuid_buf);
        send_response(client, CODE_LOGIN_SUCCESS, buf);
    } else
        create_user_and_persist(server, client, username);
    return 0;
}

void cmd_login(server_t *server, client_t *client, char *args)
{
    if (!args) {
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "Syntax error in parameters.");
        return;
    }
    if (client->state == AUTHENTICATED) {
        send_response(client, CODE_BAD_SEQUENCE,
            "Already logged in.");
        return;
    }
    if (check_arg(client, args) == -1)
        return;
    if (handle_user(client, server, args) == -1)
        return;
    send_response(client, CODE_COMMAND_OK, "Username accepted.");
}
