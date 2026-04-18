/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_create_comment.c
*/

#include "my_teams.h"
#include "logging_server.h"

static void notify_comment_created_subscribers(server_t *server,
    thread_t *thread, comment_t *comment, user_t *author)
{
    char buf[MAX_BUFFER];
    char team_uuid_str[UUID_STR_LEN];
    char thread_uuid_str[UUID_STR_LEN];
    char user_uuid_str[UUID_STR_LEN];
    user_t *receiver = NULL;

    uuid_to_string(thread->team_uuid, team_uuid_str);
    uuid_to_string(thread->uuid, thread_uuid_str);
    uuid_to_string(author->uuid, user_uuid_str);
    snprintf(buf, sizeof(buf), "%s %s %s \"%s\"", team_uuid_str,
        thread_uuid_str, user_uuid_str, comment->body);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!server->clients[i] || !server->clients[i]->username)
            continue;
        receiver = find_user_by_name(server, server->clients[i]->username);
        if (!is_user_subscribed_to_team(receiver, thread->team_uuid))
            continue;
        send_response(server->clients[i], CODE_THREAD_REPLY_CREATED_EVENT,
            buf);
    }
}

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

static void notify_comment_created(server_t *server, comment_t *comment,
    comment_notify_context_t *ctx)
{
    char thread_uuid_str[UUID_STR_LEN];
    char user_uuid_str[UUID_STR_LEN];
    char buf[MAX_BUFFER];

    uuid_to_string(ctx->thread->uuid, thread_uuid_str);
    uuid_to_string(ctx->author->uuid, user_uuid_str);
    server_event_reply_created(thread_uuid_str, user_uuid_str, comment->body);
    notify_comment_created_subscribers(server, ctx->thread, comment,
        ctx->author);
    snprintf(buf, sizeof(buf), "%s %s %ld \"%s\"", thread_uuid_str,
        user_uuid_str, (long)comment->created_at, comment->body);
    send_response(ctx->client, CODE_COMMAND_OK, buf);
}

static int store_and_notify_comment(server_t *server, client_t *client,
    comment_init_t *init)
{
    comment_t *comment = create_and_store_comment(server, init->thread_uuid,
        init->author_uuid, init->body);
    thread_t *thread = NULL;
    user_t *author = NULL;
    comment_notify_context_t notify_ctx;

    if (!comment)
        return -1;
    save_comments(&server->data.comments);
    thread = find_thread_by_uuid(server, init->thread_uuid);
    author = find_user_by_uuid(server, init->author_uuid);
    if (thread && author) {
        notify_ctx.thread = thread;
        notify_ctx.author = author;
        notify_ctx.client = client;
        notify_comment_created(server, comment, &notify_ctx);
    }
    return 0;
}

static int resolve_comment_targets(server_t *server, client_t *client,
    comment_create_params_t *tcp, comment_targets_t *targets)
{
    team_t *team = NULL;
    channel_t *channel = NULL;
    thread_context_params_t params;

    if (parse_and_verify_team(server, client, tcp->team_str, &team) != 0)
        return -1;
    if (parse_and_get_channel(server, client, tcp->channel_str, &channel) != 0)
        return -1;
    params.thread_str = tcp->thread_str;
    params.team = team;
    params.channel = channel;
    params.out_thread = &targets->thread;
    if (parse_and_get_thread_in_context(server, client, &params) != 0)
        return -1;
    if (is_user_logged_in(server, client) == 0)
        return -1;
    targets->author = find_user_by_name(server, client->username);
    if (!targets->author)
        return -1;
    return 0;
}

static int process_create_comment_tokens(server_t *server, client_t *client,
    comment_create_params_t *tcp)
{
    comment_targets_t targets;
    comment_init_t init;

    if (resolve_comment_targets(server, client, tcp, &targets) != 0)
        return -1;
    memcpy(init.thread_uuid, targets.thread->uuid, sizeof(uuid_t));
    memcpy(init.author_uuid, targets.author->uuid, sizeof(uuid_t));
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
