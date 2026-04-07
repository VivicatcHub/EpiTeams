/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** save_comments.c
*/

#include "serialization.h"

static void save_single_comment(FILE *file, comment_t *comment)
{
    char uuid_str[UUID_STR_LEN];
    char thread_uuid_str[UUID_STR_LEN];
    char author_uuid_str[UUID_STR_LEN];

    uuid_to_string(comment->uuid, uuid_str);
    uuid_to_string(comment->thread_uuid, thread_uuid_str);
    uuid_to_string(comment->author_uuid, author_uuid_str);
    fprintf(file, "\t{\n\t\t\"uuid\":\"%s\","
        "\n\t\t\"thread_uuid\":\"%s\","
        "\n\t\t\"author_uuid\":\"%s\","
        "\n\t\t\"body\":\"%s\","
        "\n\t\t\"created_at\":%ld"
        "\n\t}",
        uuid_str, thread_uuid_str, author_uuid_str, comment->body,
        comment->created_at);
}

int save_comments(struct comment_queue_s *comments)
{
    FILE *file;
    comment_t *comment;

    file = open_save_file(COMMENTS_FILE);
    if (!file)
        return -1;
    fprintf(file, "[");
    comment = TAILQ_FIRST(comments);
    if (comment)
        fprintf(file, "\n");
    while (comment) {
        save_single_comment(file, comment);
        comment = TAILQ_NEXT(comment, entries);
        if (comment)
            fprintf(file, ",\n");
    }
    fprintf(file, "\n]\n");
    fclose(file);
    return 0;
}
