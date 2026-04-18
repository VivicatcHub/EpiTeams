/*
** EPITECH PROJECT, 2026
** G-NWP-400-LYN-4-1-myteams-1
** File description:
** cmd_list_comment
*/

#include "my_teams.h"

static int parse_thread_only_args(char *args, char **out_team,
    char **out_channel, char **out_thread)
{
    *out_team = NULL;
    *out_channel = NULL;
    *out_thread = args;
    return 0;
}

static int validate_list_comments_tokens(char *channel_str, char *thread_str)
{
    if (!channel_str || channel_str[0] == '\0')
        return -1;
    if (!thread_str || thread_str[0] == '\0')
        return -1;
    return 0;
}

static int parse_list_comments_args(char *args, char **out_team,
    char **out_channel, char **out_thread)
{
    char *sep1 = NULL;
    char *sep2 = NULL;

    if (!args || args[0] == '\0')
        return -1;
    sep1 = strchr(args, ' ');
    if (!sep1)
        return parse_thread_only_args(args, out_team, out_channel,
            out_thread);
    *sep1 = '\0';
    sep2 = strchr(sep1 + 1, ' ');
    if (!sep2)
        return -1;
    *sep2 = '\0';
    *out_team = args;
    *out_channel = sep1 + 1;
    *out_thread = sep2 + 1;
    return validate_list_comments_tokens(*out_channel, *out_thread);
}

void cmd_list_comment2(client_t *client, comment_t *comment)
{
    char thread_uuid_str[UUID_STR_LEN];
    char user_uuid_str[UUID_STR_LEN];
    char buf[MAX_BUFFER];

    uuid_to_string(comment->thread_uuid, thread_uuid_str);
    uuid_to_string(comment->author_uuid, user_uuid_str);
    snprintf(buf, sizeof(buf), "%s %s %ld \"%s\"", thread_uuid_str,
        user_uuid_str, (long)comment->created_at, comment->body);
    send_response(client, CODE_COMMAND_BEGIN, buf);
}

void cmd_list_comments_for(server_t *server, client_t *client,
    thread_t *thread_ok)
{
    for (comment_t *comment = server->data.comments.tqh_first;
        comment != NULL; comment = comment->entries.tqe_next) {
        if (uuid_compare(comment->thread_uuid, thread_ok->uuid) == 0)
            cmd_list_comment2(client, comment);
    }
}

static int parse_list_comment_target(server_t *server, client_t *client,
    thread_in_channel_params_t *params)
{
    return parse_and_verify_thread_in_channel(server, client, params);
}

static int resolve_list_comments(server_t *server, client_t *client,
    char *args, thread_t **out_thread)
{
    char *team_str = NULL;
    char *channel_str = NULL;
    char *thread_str = NULL;
    team_t *team = NULL;
    channel_t *channel = NULL;
    thread_in_channel_params_t params;

    if (parse_list_comments_args(args, &team_str, &channel_str,
        &thread_str) != 0)
        return -2;
    params = (thread_in_channel_params_t){team_str, channel_str,
        thread_str, &team, &channel, out_thread};
    return parse_list_comment_target(server, client, &params);
}

void cmd_list_comments(server_t *server, client_t *client, char *args)
{
    thread_t *thread = NULL;
    int status = 0;

    if (is_user_logged_in(server, client) == 0)
        return;
    status = resolve_list_comments(server, client, args, &thread);
    if (status == -2) {
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "Syntax error in parameters.");
        return;
    }
    if (status != 0)
        return;
    cmd_list_comments_for(server, client, thread);
    send_response(client, CODE_COMMAND_END, "No more comments.");
}
