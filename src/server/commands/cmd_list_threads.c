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
        send_response(client, CODE_NOT_FOUND, "channel not found.");
        return 0;
    }
    return 1;
}


void cmd_list_threads(server_t *server, client_t *client, char *args)
{
    int max_buffer = MAX_BUFFER;
    char *result = malloc(sizeof(char) * (max_buffer + 1));
    int len = 0;
    char *uuid = malloc(sizeof(char) * (max_buffer + 1));

    memset(result, 0, max_buffer + 1);
    for (thread_t *thread = server->data.threads.tqh_first; thread != NULL; thread = thread->entries.tqe_next) {
        uuid_to_string(thread->uuid, uuid);
        len = strlen(thread->title);
        strcat(result, thread->title);
        len += 3;
        strcat(result, " | ");
        len += strlen(uuid);
        strcat(result, uuid);
        len += 3;
        strcat(result, " | ");
        len += strlen(thread->message);
        strcat(result, thread->message);
        len += 1;
        strcat(result, "\n");
        max_buffer += len;
        result = realloc(result, max_buffer);
        uuid = realloc(uuid, max_buffer);
    }
    send_response(client, CODE_COMMAND_OK, result);
    free(result);
    free(uuid);
}