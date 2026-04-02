/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cleanup_threads.c
*/

#include "serialization.h"

void cleanup_threads(struct thread_queue_s *threads)
{
    thread_t *thread;
    comment_t *comment;

    while (!TAILQ_EMPTY(threads)) {
        thread = TAILQ_FIRST(threads);
        TAILQ_REMOVE(threads, thread, entries);
        while (!TAILQ_EMPTY(&thread->comments)) {
            comment = TAILQ_FIRST(&thread->comments);
            TAILQ_REMOVE(&thread->comments, comment, entries);
            free(comment);
        }
        free(thread);
    }
}
