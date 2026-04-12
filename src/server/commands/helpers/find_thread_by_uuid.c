/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** find_thread_by_uuid.c
*/

#include "my_teams.h"

thread_t *find_thread_by_uuid(server_t *server, uuid_t uuid)
{
    thread_t *thread = NULL;

    TAILQ_FOREACH(thread, &server->data.threads, entries) {
        if (uuid_compare(thread->uuid, uuid) == 0)
            return thread;
    }
    return NULL;
}
