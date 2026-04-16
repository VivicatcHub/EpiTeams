/*
** EPITECH PROJECT, 2026
** G-NWP-400-LYN-4-1-myteams-1
** File description:
** cmd_info_thread
*/

#include "my_teams.h"

int is_channel_team_exist(server_t *server, client_t *client, char *args)
{
    uuid_t team_uuid;
    team_t *team = NULL;
    thread_t *thread = NULL;
    channel_t *channel = NULL;

    thread = find_thread_by_title(server, args);
    if (thread == NULL) {
        send_response(client, CODE_THREAD_NOT_FOUND, "Thread not found.");
        return 0;
    }
    channel = find_channel_by_uuid(server, thread->channel_uuid);
    if (channel == NULL) {
        send_response(client, CODE_CHANNEL_NOT_FOUND, "Channel not found.");
        return 0;
    }
    team = find_team_by_uuid(server, channel->team_uuid);
    if (team == NULL) {
        send_response(client, CODE_NOT_FOUND, "Team not found.");
        return 0;
    }
    return 1;
}

void cmd_info_thread(server_t *server, client_t *client, char *args)
{
    int exist = 0;
    char result[MAX_BUFFER];
    char uuid_buf[UUID_STR_LEN];
    int64_t time_;
    user_t *u;

    if (is_channel_team_exist(server, client, args) == 0)
        return;
    for (thread_t *thread = server->data.threads.tqh_first; thread != NULL;
        thread = thread->entries.tqe_next) {
        if (strcmp(args, thread->title) == 0) {
            time_ = (int64_t)thread->created_at;
            u = find_user_by_uuid(server, thread->author_uuid);
            uuid_to_string(thread->uuid, uuid_buf);
            snprintf(result, sizeof(result), "Thread name : %s | Uuid : %s\nCreated at %i by %i | Description : %s\n",
                thread->title, uuid_buf, time_, u, thread->message);
            send_response(client, CODE_COMMAND_OK, result);
            exist = 1;
        }
    }
    if (exist == 0) {
        snprintf(result, sizeof(result), "Thread name : %s doesn't exist", args);
        send_response(client, CODE_THREAD_NOT_FOUND, result);
    }
}