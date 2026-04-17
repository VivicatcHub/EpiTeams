/*
** EPITECH PROJECT, 2026
** G-NWP-400-LYN-4-1-myteams-1
** File description:
** cmd_list_comment
*/

#include "my_teams.h"

static int thread_exist(server_t *server, client_t *client, char *args)
{
    uuid_t uuid;
    thread_t *thread;
    thread_t *thread_;

    string_to_uuid(args, uuid);
    thread = find_thread_by_uuid(server, uuid);
    thread_ = find_thread_by_title(server, args);
    if (!thread && !thread_) {
        send_response(client, CODE_THREAD_NOT_FOUND, "Thread not found.");
        return 0;
    }
    return 1;
}

void cmd_list_comment2(int *len, char **result, comment_t *comment,
    int *max_buffer)
{
    char uuid[MAX_BUFFER];

    uuid_to_string(comment->uuid, uuid);
    *len += 3;
    strcat(*result, " | ");
    *len += strlen(uuid);
    strcat(*result, uuid);
    *len += 3;
    strcat(*result, " | ");
    *len += strlen(comment->body);
    strcat(*result, comment->body);
    *len += 1;
    strcat(*result, "\n");
    *max_buffer += *len;
}

void cmd_list_comments_for(server_t *server, thread_t *thread_ok, char **result, int *len)
{
    int max_buffer = MAX_BUFFER;

    for (comment_t *comment = server->data.comments.tqh_first;
        comment != NULL; comment = comment->entries.tqe_next) {
        if (uuid_compare(comment->thread_uuid, thread_ok->uuid) == 0) {
            cmd_list_comment2(len, result, comment, &max_buffer);
            *result = realloc(*result, max_buffer);
        }
    }
}

void cmd_list_comments(server_t *server, client_t *client, char *args)
{
    int len = 0;
    char *result = malloc(sizeof(char) * (MAX_BUFFER + 1));
    uuid_t uuid_args;
    thread_t *thread = find_thread_by_title(server, args);
    thread_t *thread_;
    thread_t *thread_ok;

    string_to_uuid(args, uuid_args);
    thread_ = find_thread_by_uuid(server, uuid_args);
    thread_ok = (!thread) ? thread_ : thread;
    memset(result, 0, MAX_BUFFER + 1);
    if (thread_exist(server, client, args) == 0) {
        free(result);
        return;
    }
    cmd_list_comments_for(server, thread_ok, &result, &len);
    send_response(client, CODE_COMMAND_OK, result);
    free(result);
}
