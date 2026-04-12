/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_create_comment.c
*/

#include "my_teams.h"
#include "logging_server.h"

static int parse_create_comment_args(char *args, char **out_thread,
    char **out_body)
{
    char *space = NULL;

    if (!args || args[0] == '\0')
        return -1;
    space = strchr(args, ' ');
    if (!space)
        return -1;
    *space = '\0';
    *out_thread = args;
    *out_body = space + 1;
    return 0;
}

static comment_t *create_and_store_comment(
    server_t *server,
    uuid_t thread_uuid,
    uuid_t author_uuid,
    char *body)
{
    comment_t *comment = NULL;

    comment = malloc(sizeof(comment_t));
    if (!comment)
        return NULL;
    uuid_generate(comment->uuid);
    memcpy(comment->thread_uuid, thread_uuid, sizeof(uuid_t));
    memcpy(comment->author_uuid, author_uuid, sizeof(uuid_t));
    strncpy(comment->body, body, MAX_BODY_LENGTH - 1);
    comment->body[MAX_BODY_LENGTH - 1] = '\0';
    comment->created_at = time(NULL);
    TAILQ_INSERT_TAIL(&server->data.comments, comment, entries);
    return comment;
}

static void notify_comment_created(comment_t *comment, uuid_t thread_uuid,
    user_t *author, client_t *client)
{
    char thread_uuid_str[UUID_STR_LEN];
    char user_uuid_str[UUID_STR_LEN];

    uuid_to_string(thread_uuid, thread_uuid_str);
    uuid_to_string(author->uuid, user_uuid_str);
    server_event_reply_created(thread_uuid_str, user_uuid_str,
        comment->body);
    send_response(client, CODE_COMMAND_OK, "Comment created.");
}

static thread_t *get_thread_or_respond(server_t *server,
    client_t *client, uuid_t thread_uuid)
{
    thread_t *thread = find_thread_by_uuid(server, thread_uuid);

    if (!thread) {
        send_response(client, CODE_THREAD_NOT_FOUND,
            "Thread not found.");
        return NULL;
    }
    return thread;
}

static user_t *get_author_or_respond(server_t *server, client_t *client)
{
    user_t *author = find_user_by_name(server, client->username);

    if (!author) {
        send_response(client, CODE_NOT_LOGGED_IN, "Not logged in.");
        return NULL;
    }
    return author;
}

static int store_and_notify_comment(server_t *server, client_t *client,
    comment_init_t *init)
{
    comment_t *comment = create_and_store_comment(server,
        init->thread_uuid, init->author_uuid, init->body);
    thread_t *thread = NULL;
    user_t *author = NULL;

    if (!comment)
        return -1;
    save_comments(&server->data.comments);
    thread = find_thread_by_uuid(server, init->thread_uuid);
    author = find_user_by_uuid(server, init->author_uuid);
    if (thread && author) {
        notify_comment_created(comment, thread->uuid, author, client);
    }
    return 0;
}

static int process_create_comment_tokens(
    server_t *server,
    client_t *client,
    char *thread_str,
    char *body)
{
    uuid_t thread_uuid;
    thread_t *thread = NULL;
    user_t *author = NULL;
    comment_init_t init;

    if (uuid_parse(thread_str, thread_uuid) != 0) {
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "Invalid thread uuid.");
        return -1;
    }
    thread = get_thread_or_respond(server, client, thread_uuid);
    if (!thread)
        return -1;
    author = get_author_or_respond(server, client);
    if (!author)
        return -1;
    memcpy(init.thread_uuid, thread->uuid, sizeof(uuid_t));
    memcpy(init.author_uuid, author->uuid, sizeof(uuid_t));
    init.body = body;
    return store_and_notify_comment(server, client, &init);
}

void cmd_create_comment(server_t *server, client_t *client, char *args)
{
    char *thread_str = NULL;
    char *body = NULL;

    if (parse_create_comment_args(args, &thread_str, &body) == -1) {
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "Syntax error in parameters.");
        return;
    }
    process_create_comment_tokens(server, client, thread_str, body);
}
