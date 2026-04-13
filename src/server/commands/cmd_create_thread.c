/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_create_thread.c
*/

#include "my_teams.h"
#include "logging_server.h"

static int parse_create_thread_args(char *args, char **out_channel,
    char **out_title, char **out_msg)
{
    char *s1 = NULL;
    char *s2 = NULL;

    if (!args || args[0] == '\0')
        return -1;
    s1 = strchr(args, ' ');
    if (!s1)
        return -1;
    *s1 = '\0';
    s2 = strchr(s1 + 1, ' ');
    if (!s2)
        return -1;
    *s2 = '\0';
    *out_channel = args;
    *out_title = s1 + 1;
    *out_msg = s2 + 1;
    return 0;
}

static thread_t *create_and_store_thread(server_t *server,
    thread_init_t *init)
{
    thread_t *thread = NULL;

    thread = malloc(sizeof(thread_t));
    if (!thread)
        return NULL;
    uuid_generate(thread->uuid);
    memcpy(thread->channel_uuid, init->channel_uuid, sizeof(uuid_t));
    memcpy(thread->team_uuid, init->team_uuid, sizeof(uuid_t));
    memcpy(thread->author_uuid, init->author_uuid, sizeof(uuid_t));
    strncpy(thread->title, init->title, MAX_NAME_LENGTH - 1);
    thread->title[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(thread->message, init->msg, MAX_BODY_LENGTH - 1);
    thread->message[MAX_BODY_LENGTH - 1] = '\0';
    TAILQ_INIT(&thread->comments);
    thread->created_at = time(NULL);
    TAILQ_INSERT_TAIL(&server->data.threads, thread, entries);
    return thread;
}

static void notify_thread_created(thread_t *thread, uuid_t channel_uuid,
    user_t *author, client_t *client)
{
    char channel_uuid_str[UUID_STR_LEN];
    char thread_uuid_str[UUID_STR_LEN];
    char user_uuid_str[UUID_STR_LEN];

    uuid_to_string(channel_uuid, channel_uuid_str);
    uuid_to_string(thread->uuid, thread_uuid_str);
    uuid_to_string(author->uuid, user_uuid_str);
    server_event_thread_created(channel_uuid_str, thread_uuid_str,
        user_uuid_str, thread->title, thread->message);
    send_response(client, CODE_COMMAND_OK, "Thread created.");
}

static channel_t *get_channel_or_respond(server_t *server,
    client_t *client, uuid_t chan_uuid)
{
    channel_t *channel = find_channel_by_uuid(server, chan_uuid);

    if (!channel) {
        send_response(client, CODE_CHANNEL_NOT_FOUND,
            "Channel not found.");
        return NULL;
    }
    return channel;
}

static int parse_and_get_channel(server_t *server, client_t *client,
    char *channel_str, channel_t **out_channel)
{
    uuid_t chan_uuid;

    if (string_to_uuid(channel_str, chan_uuid) != 0) {
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "Invalid channel uuid.");
        return -1;
    }
    *out_channel = get_channel_or_respond(server, client, chan_uuid);
    if (!*out_channel)
        return -1;
    return 0;
}

static user_t *get_author_or_respond_thread(server_t *server,
    client_t *client)
{
    user_t *author = find_user_by_name(server, client->username);

    if (!author) {
        send_response(client, CODE_NOT_LOGGED_IN, "Not logged in.");
        return NULL;
    }
    return author;
}

static int store_and_notify_thread(server_t *server, client_t *client,
    thread_init_t *init)
{
    thread_t *thread = create_and_store_thread(server, init);
    channel_t *channel = NULL;
    user_t *author = NULL;

    if (!thread)
        return -1;
    save_threads(&server->data.threads);
    channel = find_channel_by_uuid(server, init->channel_uuid);
    author = find_user_by_uuid(server, init->author_uuid);
    if (channel && author) {
        notify_thread_created(thread, channel->uuid, author, client);
    }
    return 0;
}

static int process_create_thread_tokens(server_t *server,
    client_t *client, thread_create_params_t *p)
{
    channel_t *channel = NULL;
    user_t *author = NULL;
    thread_init_t init;

    if (parse_and_get_channel(server, client, p->channel_str, &channel) != 0)
        return -1;
    author = get_author_or_respond_thread(server, client);
    if (!author)
        return -1;
    memcpy(init.channel_uuid, channel->uuid, sizeof(uuid_t));
    memcpy(init.team_uuid, channel->team_uuid, sizeof(uuid_t));
    memcpy(init.author_uuid, author->uuid, sizeof(uuid_t));
    init.title = p->title;
    init.msg = p->msg;
    return store_and_notify_thread(server, client, &init);
}

void cmd_create_thread(server_t *server, client_t *client, char *args)
{
    char *channel_str = NULL;
    char *title = NULL;
    char *msg = NULL;
    thread_create_params_t tcp;

    if (parse_create_thread_args(args, &channel_str, &title, &msg) == -1) {
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "Syntax error in parameters.");
        return;
    }
    tcp.channel_str = channel_str;
    tcp.title = title;
    tcp.msg = msg;
    process_create_thread_tokens(server, client, &tcp);
}
