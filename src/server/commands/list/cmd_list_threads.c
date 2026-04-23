/*
** EPITECH PROJECT, 2026
** G-NWP-400-LYN-4-1-myteams-1
** File description:
** cmd_list_thread
*/

#include "my_teams.h"

int channel_exist(server_t *server, client_t *client, char *args)
{
    uuid_t uuid;
    channel_t *channel;
    channel_t *channel_;

    string_to_uuid(args, uuid);
    channel = find_channel_by_uuid(server, uuid);
    channel_ = find_channel_by_name(server, args);
    if (!channel && !channel_) {
        send_response(client, CODE_CHANNEL_NOT_FOUND, "channel not found.");
        return 0;
    }
    return 1;
}

void cmd_list_thread2(int *len, char **result, thread_t *thread,
    int *max_buffer)
{
    char uuid[MAX_BUFFER];

    uuid_to_string(thread->uuid, uuid);
    *len = strlen(thread->title);
    strcat(*result, thread->title);
    *len += 3;
    strcat(*result, " | ");
    *len += strlen(uuid);
    strcat(*result, uuid);
    *len += 3;
    strcat(*result, " | ");
    *len += strlen(thread->message);
    strcat(*result, thread->message);
    *len += 1;
    strcat(*result, "\n");
    *max_buffer += *len;
}

void cmd_list_threads_for(server_t *server, channel_t *channel_ok,
    char **result, int *len)
{
    int max_buffer = MAX_BUFFER;

    for (thread_t *thread = server->data.threads.tqh_first;
        thread != NULL; thread = thread->entries.tqe_next) {
        if (uuid_compare(thread->channel_uuid, channel_ok->uuid) == 0) {
            cmd_list_thread2(len, result, thread, &max_buffer);
            *result = realloc(*result, max_buffer);
        }
    }
}

static int cmd_list_threads_check_channel(server_t *server, client_t *client,
    char *args, channel_t **channel_ok)
{
    uuid_t uuid_args;
    channel_t *channel = find_channel_by_name(server, args);
    user_t *user = find_user_by_name(server, client->username);

    string_to_uuid(args, uuid_args);
    *channel_ok = (!channel) ? find_channel_by_uuid(server, uuid_args) :
        channel;
    if (channel_exist(server, client, args) == 0)
        return 0;
    if (require_user_subscribed_to_team(server, client, user,
        find_team_by_uuid(server, (*channel_ok)->team_uuid)) != 0)
        return 0;
    return 1;
}

void cmd_list_threads(server_t *server, client_t *client, char *args)
{
    int len = 0;
    char *result = malloc(sizeof(char) * (MAX_BUFFER + 1));
    channel_t *channel_ok = NULL;

    if (!result)
        return;
    memset(result, 0, MAX_BUFFER + 1);
    if (!cmd_list_threads_check_channel(server, client, args, &channel_ok)) {
        free(result);
        return;
    }
    cmd_list_threads_for(server, channel_ok, &result, &len);
    send_response(client, CODE_COMMAND_OK, result);
    free(result);
}
