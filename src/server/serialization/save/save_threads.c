/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** save_threads.c
*/

#include "serialization.h"

static void save_single_thread(FILE *file, thread_t *thread)
{
    char uuid_str[UUID_STR_LEN];
    char channel_uuid_str[UUID_STR_LEN];
    char team_uuid_str[UUID_STR_LEN];
    char author_uuid_str[UUID_STR_LEN];

    uuid_to_string(thread->uuid, uuid_str);
    uuid_to_string(thread->channel_uuid, channel_uuid_str);
    uuid_to_string(thread->team_uuid, team_uuid_str);
    uuid_to_string(thread->author_uuid, author_uuid_str);
    fprintf(file, "\t{\n\t\t\"uuid\":\"%s\","
        "\n\t\t\"channel_uuid\":\"%s\","
        "\n\t\t\"team_uuid\":\"%s\","
        "\n\t\t\"author_uuid\":\"%s\","
        "\n\t\t\"title\":\"%s\","
        "\n\t\t\"message\":\"%s\","
        "\n\t\t\"created_at\":%ld"
        "\n\t}",
        uuid_str, channel_uuid_str, team_uuid_str, author_uuid_str,
        thread->title, thread->message, thread->created_at);
}

int save_threads(struct thread_queue_s *threads)
{
    FILE *file;
    thread_t *thread;

    file = open_save_file(THREADS_FILE);
    if (!file)
        return -1;
    fprintf(file, "[");
    thread = TAILQ_FIRST(threads);
    if (thread)
        fprintf(file, "\n");
    while (thread) {
        save_single_thread(file, thread);
        thread = TAILQ_NEXT(thread, entries);
        if (thread)
            fprintf(file, ",\n");
    }
    fprintf(file, "\n]\n");
    fclose(file);
    return 0;
}
