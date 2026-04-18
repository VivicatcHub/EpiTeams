/*
** EPITECH PROJECT, 2026
** G-NWP-400-LYN-4-1-myteams-1
** File description:
** cmd_list_thread
*/

#include "my_teams.h"

static int parse_list_threads_args(char *args, char **out_team,
    char **out_channel)
{
    char *sep = NULL;

    if (!args || args[0] == '\0')
        return -1;
    sep = strchr(args, ' ');
    if (!sep) {
        *out_team = NULL;
        *out_channel = args;
        return 0;
    }
    *sep = '\0';
    *out_team = args;
    *out_channel = sep + 1;
    if (!*out_channel || (*out_channel)[0] == '\0')
        return -1;
    return 0;
}

void cmd_list_thread2(client_t *client, thread_t *thread)
{
    char thread_uuid_str[UUID_STR_LEN];
    char user_uuid_str[UUID_STR_LEN];
    char buf[MAX_BUFFER];

    uuid_to_string(thread->uuid, thread_uuid_str);
    uuid_to_string(thread->author_uuid, user_uuid_str);
    snprintf(buf, sizeof(buf), "%s %s %ld \"%s\" \"%s\"", thread_uuid_str,
        user_uuid_str, (long)thread->created_at, thread->title,
        thread->message);
    send_response(client, CODE_COMMAND_BEGIN, buf);
}

void cmd_list_threads_for(server_t *server, client_t *client,
    channel_t *channel_ok)
{
    for (thread_t *thread = server->data.threads.tqh_first;
        thread != NULL; thread = thread->entries.tqe_next) {
        if (uuid_compare(thread->channel_uuid, channel_ok->uuid) == 0)
            cmd_list_thread2(client, thread);
    }
}

static int resolve_list_threads_channel(server_t *server, client_t *client,
    char *args, channel_t **out_channel)
{
    char *team_str = NULL;
    char *channel_str = NULL;
    team_t *team = NULL;
    channel_in_team_params_t params;

    if (parse_list_threads_args(args, &team_str, &channel_str) != 0)
        return -2;
    params = (channel_in_team_params_t){team_str, channel_str,
        &team, out_channel};
    return parse_and_verify_channel_in_team(server, client, &params);
}

void cmd_list_threads(server_t *server, client_t *client, char *args)
{
    channel_t *channel = NULL;
    int status = 0;

    if (is_user_logged_in(server, client) == 0)
        return;
    status = resolve_list_threads_channel(server, client, args, &channel);
    if (status == -2) {
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "Syntax error in parameters.");
        return;
    }
    if (status != 0)
        return;
    cmd_list_threads_for(server, client, channel);
    send_response(client, CODE_COMMAND_END, "No more threads.");
}
