/*
** EPITECH PROJECT, 2026
** G-NWP-400-LYN-4-1-myteams-1
** File description:
** cmd_info_thread
*/

#include "my_teams.h"

static int parse_info_thread_args(char *args, char **out_team,
    char **out_channel, char **out_thread)
{
    char *sep1 = NULL;
    char *sep2 = NULL;

    if (!args || args[0] == '\0')
        return -1;
    sep1 = strchr(args, ' ');
    if (!sep1)
        return -1;
    *sep1 = '\0';
    sep2 = strchr(sep1 + 1, ' ');
    if (!sep2)
        return -1;
    *sep2 = '\0';
    *out_team = args;
    *out_channel = sep1 + 1;
    *out_thread = sep2 + 1;
    return 0;
}

static int verify_team_channel_thread(server_t *server, client_t *client,
    thread_in_channel_params_t *params)
{
    return parse_and_verify_thread_in_channel(server, client, params);
}

void cmd_info_thread_2(client_t *client, thread_t *thread_ok)
{
    char thread_uuid_buf[UUID_STR_LEN];
    char author_uuid_buf[UUID_STR_LEN];
    char result[MAX_BUFFER];

    uuid_to_string(thread_ok->uuid, thread_uuid_buf);
    uuid_to_string(thread_ok->author_uuid, author_uuid_buf);
    snprintf(result, sizeof(result), "%s %s %ld \"%s\" \"%s\"",
        thread_uuid_buf, author_uuid_buf, (long)thread_ok->created_at,
        thread_ok->title, thread_ok->message);
    send_response(client, CODE_COMMAND_BEGIN, result);
    send_response(client, CODE_COMMAND_END, "No more thread info.");
}

static int resolve_info_thread(server_t *server, client_t *client,
    char *args, thread_t **out_thread)
{
    char *team_str = NULL;
    char *channel_str = NULL;
    char *thread_str = NULL;
    team_t *team = NULL;
    channel_t *channel = NULL;
    thread_in_channel_params_t params;

    if (parse_info_thread_args(args, &team_str, &channel_str,
        &thread_str) != 0)
        return -2;
    params = (thread_in_channel_params_t){team_str, channel_str,
        thread_str, &team, &channel, out_thread};
    return verify_team_channel_thread(server, client, &params);
}

void cmd_info_thread(server_t *server, client_t *client, char *args)
{
    thread_t *thread = NULL;
    int status = 0;

    status = resolve_info_thread(server, client, args, &thread);
    if (status == -2) {
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "Syntax error in parameters.");
        return;
    }
    if (status != 0)
        return;
    cmd_info_thread_2(client, thread);
}
