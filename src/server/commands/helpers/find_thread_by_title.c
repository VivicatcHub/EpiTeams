/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** find_thread_by_title.c
*/

#include "my_teams.h"

thread_t *find_thread_by_title(server_t *server, char const *title)
{
    thread_t *thread = NULL;

    TAILQ_FOREACH(thread, &server->data.threads, entries) {
        if (strcmp(thread->title, title) == 0)
            return thread;
    }
    return NULL;
}
