/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_create_comment.c
*/

#include "my_teams.h"
#include "logging_server.h"

static int parse_create_comment_args(char *args,
    comment_create_params_t **out_params)
{
    (*out_params)->team_str = strtok(args, " ");
    (*out_params)->channel_str = strtok(NULL, " ");
    (*out_params)->thread_str = strtok(NULL, " ");
    (*out_params)->body = strtok(NULL, "");
    if (!(*out_params)->team_str || !(*out_params)->channel_str ||
        !(*out_params)->thread_str || !(*out_params)->body)
        return -1;
    return 0;
}

static comment_t *create_and_store_comment(server_t *server,
    uuid_t thread_uuid, uuid_t author_uuid, char *body)
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
    server_event_reply_created(thread_uuid_str, user_uuid_str, comment->body);
    send_response(client, CODE_COMMAND_OK, "Comment created.");
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
    comment_t *comment = create_and_store_comment(server, init->thread_uuid,
        init->author_uuid, init->body);
    thread_t *thread = NULL;
    user_t *author = NULL;

    if (!comment)
        return -1;
    save_comments(&server->data.comments);
    thread = find_thread_by_uuid(server, init->thread_uuid);
    author = find_user_by_uuid(server, init->author_uuid);
    if (thread && author)
        notify_comment_created(comment, thread->uuid, author, client);
    return 0;
}

static int process_create_comment_tokens(server_t *server, client_t *client,
    comment_create_params_t *tcp)
{
    team_t *team = NULL;
    channel_t *channel = NULL;
    thread_t *thread = NULL;
    user_t *author = NULL;
    comment_init_t init;

    if (parse_and_verify_team(server, client, tcp->team_str, &team) != 0)
        return -1;
    if (parse_and_get_channel(server, client, tcp->channel_str, &channel) != 0)
        return -1;
    if (parse_and_get_thread(server, client, tcp->thread_str, &thread) != 0)
        return -1;
    author = get_author_or_respond(server, client);
    if (!author)
        return -1;
    memcpy(init.thread_uuid, thread->uuid, sizeof(uuid_t));
    memcpy(init.author_uuid, author->uuid, sizeof(uuid_t));
    init.body = tcp->body;
    return store_and_notify_comment(server, client, &init);
}

void cmd_create_comment(server_t *server, client_t *client, char *args)
{
    comment_create_params_t *tcp = malloc(sizeof(comment_create_params_t));

    if (!tcp) {
        send_response(client, CODE_SYNTAX_ERROR_PARAM, "Internal error.");
        return;
    }
    if (parse_create_comment_args(args, &tcp) == -1) {
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "Syntax error in parameters.");
        free(tcp);
        return;
    }
    process_create_comment_tokens(server, client, tcp);
    free(tcp);
}
