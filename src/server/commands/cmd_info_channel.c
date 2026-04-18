/*
** EPITECH PROJECT, 2026
** G-NWP-400-LYN-4-1-myteams-1
** File description:
** cmd_info_channel
*/

#include "my_teams.h"

static int parse_info_channel_args(char *args, char **out_team,
    char **out_channel)
{
    char *sep = NULL;

    if (!args || args[0] == '\0')
        return -1;
    sep = strchr(args, ' ');
    if (!sep)
        return -1;
    *sep = '\0';
    *out_team = args;
    *out_channel = sep + 1;
    if (!*out_channel || (*out_channel)[0] == '\0')
        return -1;
    return 0;
}

static int verify_team_channel(server_t *server, client_t *client,
    channel_in_team_params_t *params)
{
    return parse_and_verify_channel_in_team(server, client, params);
}

void cmd_info_channel_2(client_t *client, channel_t *channel_ok)
{
    char uuid_buf[UUID_STR_LEN];
    char result[MAX_BUFFER];

    uuid_to_string(channel_ok->uuid, uuid_buf);
    snprintf(result, sizeof(result), "%s (%s) - %s", channel_ok->name,
        uuid_buf, channel_ok->description);
    send_response(client, CODE_COMMAND_BEGIN, result);
    send_response(client, CODE_COMMAND_END, "No more channel info.");
}

void cmd_info_channel(server_t *server, client_t *client, char *args)
{
    char *team_str = NULL;
    char *channel_str = NULL;
    team_t *team = NULL;
    channel_t *channel = NULL;
    channel_in_team_params_t params;

    if (parse_info_channel_args(args, &team_str, &channel_str) != 0) {
        send_response(client, CODE_SYNTAX_ERROR_PARAM,
            "Syntax error in parameters.");
        return;
    }
    params.team_str = team_str;
    params.channel_str = channel_str;
    params.out_team = &team;
    params.out_channel = &channel;
    if (verify_team_channel(server, client, &params) != 0)
        return;
    cmd_info_channel_2(client, channel);
}
