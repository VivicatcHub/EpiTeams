/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_send.c
*/

#include "my_teams.h"


static int resolve_identifier_to_uuid(server_t *server, char *token,
    uuid_t target)
{
    user_t *user = NULL;

    if (string_to_uuid(token, target) == 0)
        return 0;
    user = find_user_by_name(server, token);
    if (!user)
        return -1;
    memcpy(target, user->uuid, sizeof(uuid_t));
    return 0;
}

static int parse_send_args(server_t *server, char *args, uuid_t target,
    char **out_body)
{
    char *space = NULL;

    if (!args || args[0] == '\0')
        return -1;
    space = strchr(args, ' ');
    if (!space)
        return -1;
    *space = '\0';
    *out_body = space + 1;
    if (resolve_identifier_to_uuid(server, args, target) != 0)
        return -1;
    return 0;
}

static private_message_t *create_and_store_message(server_t *server,
    user_t *sender, user_t *receiver, char *body)
{
    private_message_t *msg = malloc(sizeof(private_message_t));

    if (!msg)
        return NULL;
    uuid_generate(msg->uuid);
    memcpy(msg->sender_uuid, sender->uuid, sizeof(uuid_t));
    memcpy(msg->receiver_uuid, receiver->uuid, sizeof(uuid_t));
    strncpy(msg->body, body, MAX_BODY_LENGTH - 1);
    msg->body[MAX_BODY_LENGTH - 1] = '\0';
    msg->created_at = time(NULL);
    TAILQ_INSERT_TAIL(&server->data.messages, msg, entries);
    return msg;
}

static void notify_connected_receiver(server_t *server, user_t *receiver,
    user_t *sender, char const *message_body)
{
    char buf[MAX_BUFFER];

    if (!receiver)
        return;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!server->clients[i])
            continue;
        if (!server->clients[i]->username)
            continue;
        if (strcmp(server->clients[i]->username, receiver->name) == 0) {
            snprintf(buf, MAX_BUFFER, "New message from %s : \"%s\"",
                sender->name, message_body);
            send_response(server->clients[i], CODE_MESSAGE_RECEIVED, buf);
            return;
        }
    }
}

static void notify_and_respond(server_t *server, client_t *client,
    user_t *sender, private_message_t *msg)
{
    char sender_uuid_str[UUID_STR_LEN];
    char receiver_uuid_str[UUID_STR_LEN];
    user_t *receiver = find_user_by_uuid(server, msg->receiver_uuid);

    save_messages(&server->data.messages);
    uuid_to_string(sender->uuid, sender_uuid_str);
    uuid_to_string(msg->receiver_uuid, receiver_uuid_str);
    server_event_private_message_sended(sender_uuid_str, receiver_uuid_str,
        msg->body);
    notify_connected_receiver(server, receiver, sender, msg->body);
    send_response(client, CODE_COMMAND_OK, "Message sent.");
    send_response(client, CODE_COMMAND_END, "End of message.");
}

static user_t *get_receiver_or_respond(server_t *server, client_t *client,
    uuid_t target)
{
    user_t *receiver = find_user_by_uuid(server, target);

    if (!receiver)
        send_response(client, CODE_NOT_FOUND, "User not found.");
    return receiver;
}

static user_t *get_sender_or_respond(server_t *server, client_t *client)
{
    user_t *sender = find_user_by_name(server, client->username);

    if (!sender)
        send_response(client, CODE_NOT_LOGGED_IN, "Not logged in.");
    return sender;
}

void cmd_send(server_t *server, client_t *client, char *args)
{
    uuid_t target;
    char *msg_body = NULL;
    user_t *receiver = NULL;
    user_t *sender = NULL;
    private_message_t *message = NULL;

    if (parse_send_args(server, args, target, &msg_body) == -1) {
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "Syntax error in parameters.");
        return;
    }
    receiver = get_receiver_or_respond(server, client, target);
    sender = get_sender_or_respond(server, client);
    if (!receiver || !sender)
        return;
    message = create_and_store_message(server, sender, receiver, msg_body);
    if (!message)
        return;
    notify_and_respond(server, client, sender, message);
}
