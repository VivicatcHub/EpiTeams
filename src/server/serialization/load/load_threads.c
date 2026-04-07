/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** load_threads.c
*/

#include "serialization.h"

static int load_single_thread(FILE *file, char *uuid_str, thread_t *thread)
{
    char channel_uuid_str[UUID_STR_LEN];
    char team_uuid_str[UUID_STR_LEN];
    char author_uuid_str[UUID_STR_LEN];
    long created_at;

    string_to_uuid(uuid_str, thread->uuid);
    fscanf(file, ",\n\t\t\"channel_uuid\":\"%36[^\"]\"", channel_uuid_str);
    string_to_uuid(channel_uuid_str, thread->channel_uuid);
    fscanf(file, ",\n\t\t\"team_uuid\":\"%36[^\"]\"", team_uuid_str);
    string_to_uuid(team_uuid_str, thread->team_uuid);
    fscanf(file, ",\n\t\t\"author_uuid\":\"%36[^\"]\"", author_uuid_str);
    string_to_uuid(author_uuid_str, thread->author_uuid);
    fscanf(file, ",\n\t\t\"title\":\"%31[^\"]\"", thread->title);
    fscanf(file, ",\n\t\t\"message\":\"%511[^\"]\"", thread->message);
    fscanf(file, ",\n\t\t\"created_at\":%ld", &created_at);
    thread->created_at = (time_t)created_at;
    TAILQ_INIT(&thread->comments);
    fscanf(file, "\n\t}");
    return 0;
}

static int append_thread(struct thread_queue_s *threads, FILE *file,
    char *uuid_str)
{
    thread_t *thread;

    thread = malloc(sizeof(thread_t));
    if (!thread)
        return -1;
    if (load_single_thread(file, uuid_str, thread) == -1) {
        free(thread);
        return -1;
    }
    TAILQ_INSERT_TAIL(threads, thread, entries);
    return 0;
}

int load_threads(struct thread_queue_s *threads)
{
    FILE *file;
    char uuid_str[UUID_STR_LEN];

    TAILQ_INIT(threads);
    file = open_load_file(THREADS_FILE);
    if (!file)
        return -1;
    printf("Loading threads...\n");
    fscanf(file, "[");
    while (fscanf(file, "\n\t{\n\t\t\"uuid\":\"%36[^\"]\"", uuid_str) == 1) {
        if (append_thread(threads, file, uuid_str) == -1) {
            fclose(file);
            return -1;
        }
        fscanf(file, ",");
    }
    fclose(file);
    return 0;
}
