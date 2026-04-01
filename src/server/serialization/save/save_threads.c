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
    fprintf(file, "{\"uuid\":\"%s\",\"channel_uuid\":\"%s\","
        "\"team_uuid\":\"%s\",\"author_uuid\":\"%s\",\"title\":\"%s\","
        "\"message\":\"%s\",\"created_at\":%ld}",
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
    while (thread) {
        save_single_thread(file, thread);
        thread = TAILQ_NEXT(thread, entries);
        if (thread)
            fprintf(file, ",");
    }
    fprintf(file, "]\n");
    fclose(file);
    return 0;
}
