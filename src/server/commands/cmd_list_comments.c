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

void cmd_list_comments(server_t *server, client_t *client, char *args)
{
    int max_buffer = MAX_BUFFER;
    char *result = malloc(sizeof(char) * (max_buffer + 1));
    int len = 0;
    char *uuid = malloc(sizeof(char) * (max_buffer + 1));
    user_t *author;

    if (thread_exist(server, client, args) == 0)
        return;
    memset(result, 0, max_buffer + 1);
    for (comment_t *comment = server->data.comments.tqh_first; comment != NULL; comment = comment->entries.tqe_next) {
        uuid_to_string(comment->uuid, uuid);
        author = find_user_by_uuid(server, comment->author_uuid);
        len = strlen(author->name);
        strcat(result, author->name);
        len += 3;
        strcat(result, " | ");
        len += strlen(uuid);
        strcat(result, uuid);
        len += 3;
        strcat(result, " | ");
        len += strlen(comment->body);
        strcat(result, comment->body);
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