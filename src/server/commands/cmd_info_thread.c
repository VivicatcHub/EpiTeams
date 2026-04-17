/*
** EPITECH PROJECT, 2026
** G-NWP-400-LYN-4-1-myteams-1
** File description:
** cmd_info_thread
*/

#include "my_teams.h"

int is_channel_exist(server_t *server, client_t *client, char *args)
{
    channel_t *channel = NULL;
    thread_t *thread = NULL;
    thread_t *thread_n = NULL;
    thread_t *thread_ok = NULL;
    uuid_t uuid;

    string_to_uuid(args, uuid);
    thread = find_thread_by_uuid(server, uuid);
    thread_n = find_thread_by_name(server, args);
    if (!thread && !thread_n) {
        send_response(client, CODE_THREAD_NOT_FOUND, "Thread not found.");
        return 0;
    }
    thread_ok = (!thread) ? thread_n : thread;
    channel = find_channel_by_uuid(server, thread_ok->channel_uuid);
    if (!channel) {
        send_response(client, CODE_CHANNEL_NOT_FOUND, "Channel not found.");
        return 0;
    }
    return 1;
}

void cmd_info_thread_2(client_t *client, thread_t *thread_ok)
{
    char uuid_buf[UUID_STR_LEN];
    char result[MAX_BUFFER];
    int64_t time_;

    time_ = (int64_t)thread_ok->created_at;
    uuid_to_string(thread_ok->uuid, uuid_buf);
    snprintf(result, sizeof(result),
        "thread Name : %s | Uuid : %s\n Created at %i | Description : %s",
        thread_ok->title, uuid_buf, time_, thread_ok->message);
    send_response(client, CODE_COMMAND_OK, result);
}

void cmd_info_thread(server_t *server, client_t *client, char *args)
{
    thread_t *thread;
    thread_t *thread_n;
    thread_t *thread_ok;
    uuid_t uuid;

    if (is_channel_exist(server, client, args) == 0)
        return;
    string_to_uuid(args, uuid);
    thread = find_thread_by_uuid(server, uuid);
    thread_n = find_thread_by_name(server, args);
    if (!thread && !thread_n) {
        send_response(client, CODE_NOT_FOUND, "thread not found.");
        return;
    }
    thread_ok = (!thread) ? thread_n : thread;
    cmd_info_thread_2(client, thread_ok);
}
