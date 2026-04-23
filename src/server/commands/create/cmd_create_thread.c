/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_create_thread.c
*/

#include "my_teams.h"
#include "logging_server.h"

static int parse_create_thread_args(char *args,
    thread_create_params_t **out_params)
{
    (*out_params)->team_str = strtok(args, " ");
    (*out_params)->channel_str = strtok(NULL, " ");
    (*out_params)->title = strtok(NULL, " ");
    (*out_params)->msg = strtok(NULL, "");
    if (!(*out_params)->team_str || !(*out_params)->channel_str ||
        !(*out_params)->title || !(*out_params)->msg)
        return -1;
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
    char response[MAX_BUFFER];

    uuid_to_string(channel_uuid, channel_uuid_str);
    uuid_to_string(thread->uuid, thread_uuid_str);
    uuid_to_string(author->uuid, user_uuid_str);
    server_event_thread_created(channel_uuid_str, thread_uuid_str,
        user_uuid_str, thread->title, thread->message);
    snprintf(response, sizeof(response), "%s %s %ld \"%s\" \"%s\"",
        thread_uuid_str, user_uuid_str, (long)thread->created_at,
        thread->title, thread->message);
    send_response(client, CODE_COMMAND_OK, response);
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

static int resolve_thread_context(server_t *server, client_t *client,
    thread_create_params_t *p, thread_prepare_ctx_t *ctx)
{
    channel_in_team_params_t params = {
        .team_str = p->team_str,
        .channel_str = p->channel_str,
        .out_team = &ctx->team,
        .out_channel = &ctx->channel,
    };

    if (parse_and_verify_channel_in_team(server, client, &params) != 0)
        return -1;
    ctx->author = get_author_or_respond_thread(server, client);
    if (!ctx->author || require_user_subscribed_to_team(server, client,
        ctx->author, ctx->team) != 0)
        return -1;
    return 0;
}

static void fill_thread_init(thread_init_t *init, thread_create_params_t *p,
    thread_prepare_ctx_t *ctx)
{
    memcpy(init->channel_uuid, ctx->channel->uuid, sizeof(uuid_t));
    memcpy(init->team_uuid, ctx->channel->team_uuid, sizeof(uuid_t));
    memcpy(init->author_uuid, ctx->author->uuid, sizeof(uuid_t));
    init->title = p->title;
    init->msg = p->msg;
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

static int prepare_thread_init(server_t *server, client_t *client,
    thread_create_params_t *p, thread_init_t *init)
{
    thread_prepare_ctx_t ctx = {0};

    if (resolve_thread_context(server, client, p, &ctx) != 0)
        return -1;
    fill_thread_init(init, p, &ctx);
    return 0;
}

static int process_create_thread_tokens(server_t *server,
    client_t *client, thread_create_params_t *p)
{
    thread_init_t init;

    if (prepare_thread_init(server, client, p, &init) != 0)
        return -1;
    return store_and_notify_thread(server, client, &init);
}

void cmd_create_thread(server_t *server, client_t *client, char *args)
{
    thread_create_params_t *tcp = malloc(sizeof(thread_create_params_t));

    if (!tcp) {
        send_response(client, CODE_SYNTAX_ERROR_PARAM, "Internal error.");
        return;
    }
    if (parse_create_thread_args(args, &tcp) == -1) {
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "Syntax error in parameters.");
        free(tcp);
        return;
    }
    process_create_thread_tokens(server, client, tcp);
    free(tcp);
}
