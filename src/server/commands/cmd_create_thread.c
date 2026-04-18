/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_create_thread.c
*/

#include "my_teams.h"
#include "logging_server.h"

static int thread_title_exists_in_channel(server_t *server,
    uuid_t channel_uuid,
    char const *title)
{
    thread_t *thread = NULL;

    TAILQ_FOREACH(thread, &server->data.threads, entries) {
        if (uuid_compare(thread->channel_uuid, channel_uuid) != 0)
            continue;
        if (strcmp(thread->title, title) == 0)
            return 1;
    }
    return 0;
}

static void notify_thread_created_subscribers(server_t *server,
    thread_t *thread, user_t *author)
{
    char buf[MAX_BUFFER];
    char thread_uuid_str[UUID_STR_LEN];
    char user_uuid_str[UUID_STR_LEN];
    user_t *receiver = NULL;

    uuid_to_string(thread->uuid, thread_uuid_str);
    uuid_to_string(author->uuid, user_uuid_str);
    snprintf(buf, sizeof(buf), "%s %s %ld \"%s\" \"%s\"", thread_uuid_str,
        user_uuid_str, (long)thread->created_at, thread->title,
        thread->message);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!server->clients[i] || !server->clients[i]->username)
            continue;
        receiver = find_user_by_name(server, server->clients[i]->username);
        if (!is_user_subscribed_to_team(receiver, thread->team_uuid))
            continue;
        send_response(server->clients[i], CODE_THREAD_CREATED_EVENT, buf);
    }
}

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

static void notify_thread_created(server_t *server, thread_t *thread,
    thread_notify_context_t *ctx)
{
    char channel_uuid_str[UUID_STR_LEN];
    char thread_uuid_str[UUID_STR_LEN];
    char user_uuid_str[UUID_STR_LEN];
    char buf[MAX_BUFFER];

    uuid_to_string(ctx->channel_uuid, channel_uuid_str);
    uuid_to_string(thread->uuid, thread_uuid_str);
    uuid_to_string(ctx->author->uuid, user_uuid_str);
    server_event_thread_created(channel_uuid_str, thread_uuid_str,
        user_uuid_str, thread->title, thread->message);
    notify_thread_created_subscribers(server, thread, ctx->author);
    snprintf(buf, sizeof(buf), "%s %s %ld \"%s\" \"%s\"",
        thread_uuid_str, user_uuid_str, (long)thread->created_at,
        thread->title, thread->message);
    send_response(ctx->client, CODE_COMMAND_OK, buf);
}

static int store_and_notify_thread(server_t *server, client_t *client,
    thread_init_t *init)
{
    thread_t *thread = create_and_store_thread(server, init);
    channel_t *channel = NULL;
    user_t *author = NULL;
    thread_notify_context_t notify_ctx;

    if (!thread)
        return -1;
    save_threads(&server->data.threads);
    channel = find_channel_by_uuid(server, init->channel_uuid);
    author = find_user_by_uuid(server, init->author_uuid);
    if (channel && author) {
        memcpy(notify_ctx.channel_uuid, channel->uuid, sizeof(uuid_t));
        notify_ctx.author = author;
        notify_ctx.client = client;
        notify_thread_created(server, thread, &notify_ctx);
    }
    return 0;
}

static int resolve_thread_targets(server_t *server, client_t *client,
    thread_create_params_t *p, thread_targets_t *targets)
{
    team_t *team = NULL;

    if (parse_and_verify_team(server, client, p->team_str, &team) != 0)
        return -1;
    if (parse_and_get_channel(server, client, p->channel_str,
        &targets->channel) != 0)
        return -1;
    if (is_user_logged_in(server, client) == 0)
        return -1;
    targets->author = find_user_by_name(server, client->username);
    if (!targets->author)
        return -1;
    return 0;
}

static int process_create_thread_tokens(server_t *server,
    client_t *client, thread_create_params_t *p)
{
    thread_targets_t targets;
    thread_init_t init;

    if (resolve_thread_targets(server, client, p, &targets) != 0)
        return -1;
    if (thread_title_exists_in_channel(server, targets.channel->uuid,
        p->title)) {
        send_response(client, CODE_ALREADY_SUBSCRIBED,
            "Thread already exists.");
        return -1;
    }
    memcpy(init.channel_uuid, targets.channel->uuid, sizeof(uuid_t));
    memcpy(init.team_uuid, targets.channel->team_uuid, sizeof(uuid_t));
    memcpy(init.author_uuid, targets.author->uuid, sizeof(uuid_t));
    init.title = p->title;
    init.msg = p->msg;
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
