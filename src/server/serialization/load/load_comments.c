/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** load_comments.c
*/

#include "serialization.h"

static int load_single_comment(FILE *file, char *uuid_str, comment_t *comment)
{
    char thread_uuid_str[UUID_STR_LEN];
    char author_uuid_str[UUID_STR_LEN];
    long created_at;

    string_to_uuid(uuid_str, comment->uuid);
    fscanf(file, ",\"thread_uuid\":\"%36[^\"]\"", thread_uuid_str);
    string_to_uuid(thread_uuid_str, comment->thread_uuid);
    fscanf(file, ",\"author_uuid\":\"%36[^\"]\"", author_uuid_str);
    string_to_uuid(author_uuid_str, comment->author_uuid);
    fscanf(file, ",\"body\":\"%511[^\"]\"", comment->body);
    fscanf(file, ",\"created_at\":%ld", &created_at);
    comment->created_at = (time_t)created_at;
    fscanf(file, "}");
    return 0;
}

static int append_comment(struct comment_queue_s *comments, FILE *file,
    char *uuid_str)
{
    comment_t *comment;

    comment = malloc(sizeof(comment_t));
    if (!comment)
        return -1;
    if (load_single_comment(file, uuid_str, comment) == -1) {
        free(comment);
        return -1;
    }
    TAILQ_INSERT_TAIL(comments, comment, entries);
    return 0;
}

int load_comments(struct comment_queue_s *comments)
{
    FILE *file;
    char uuid_str[UUID_STR_LEN];

    TAILQ_INIT(comments);
    file = open_load_file(COMMENTS_FILE);
    if (!file)
        return 0;
    printf("Loading comments...\n");
    fscanf(file, "[");
    while (fscanf(file, "{\"uuid\":\"%36[^\"]\"", uuid_str) == 1) {
        if (append_comment(comments, file, uuid_str) == -1) {
            fclose(file);
            return -1;
        }
        fscanf(file, ",");
    }
    fclose(file);
    return 0;
}
