/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_messages.c
*/

#include "my_teams.h"

static int find_receiver_by_args(server_t *server, char *args,
    user_t **receiver)
{
    uuid_t target;

    if (!args || args[0] == '\0')
        return -1;
    if (string_to_uuid(args, target) == 0) {
        *receiver = find_user_by_uuid(server, target);
        return (*receiver != NULL) ? 0 : 1;
    }
    *receiver = find_user_by_name(server, args);
    return (*receiver != NULL) ? 0 : 1;
}

static int find_current_user(server_t *server, client_t *client,
    user_t **current_user)
{
    if (!client || !client->username)
        return -1;
    *current_user = find_user_by_name(server, client->username);
    return (*current_user != NULL) ? 0 : -1;
}

static void send_matching_message(server_t *server, client_t *client,
    private_message_t *m)
{
    user_t *sender = find_user_by_uuid(server, m->sender_uuid);
    const char *sender_name = (sender && sender->name[0]) ? sender->name :
        "Deleted User";
    char timebuf[21];
    char buf[MAX_BUFFER];
    time_t t = (time_t)m->created_at;
    struct tm tm = *localtime(&t);

    if (strftime(timebuf, sizeof(timebuf), "%d/%m/%y %I:%M %p", &tm) == 0)
        strncpy(timebuf, "1/1/70, 00:00 AM", sizeof(timebuf));
    snprintf(buf, MAX_BUFFER, "%s: %s (%s)", sender_name, m->body, timebuf);
    send_response(client, CODE_COMMAND_BEGIN, buf);
}

static void send_conversation_messages(server_t *server, client_t *client,
    user_t *sender, user_t *receiver)
{
    private_message_t *message = NULL;

    TAILQ_FOREACH(message, &server->data.messages, entries) {
        if ((uuid_compare(message->sender_uuid, sender->uuid) == 0 &&
            uuid_compare(message->receiver_uuid, receiver->uuid) == 0) ||
            (uuid_compare(message->sender_uuid, receiver->uuid) == 0 &&
                uuid_compare(message->receiver_uuid, sender->uuid) == 0)) {
            send_matching_message(server, client, message);
        }
    }
    send_response(client, CODE_COMMAND_END, "End of messages.");
}

void cmd_messages(server_t *server, client_t *client, char *args)
{
    user_t *receiver = NULL;
    user_t *current_user = NULL;

    switch (find_receiver_by_args(server, args, &receiver)) {
    case -1:
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "Syntax error in parameters.");
        return;
    case 1:
        send_response(client, CODE_NOT_FOUND, "User not found.");
        return;
    }
    if (find_current_user(server, client, &current_user) != 0) {
        send_response(client, CODE_NOT_LOGGED_IN, "Not logged in.");
        return;
    }
    send_conversation_messages(server, client, current_user, receiver);
}
