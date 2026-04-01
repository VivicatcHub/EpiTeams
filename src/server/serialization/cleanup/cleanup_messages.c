/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cleanup_messages.c
*/

#include "serialization.h"

void cleanup_messages(struct message_queue_s *messages)
{
    private_message_t *msg;

    while (!TAILQ_EMPTY(messages)) {
        msg = TAILQ_FIRST(messages);
        TAILQ_REMOVE(messages, msg, entries);
        free(msg);
    }
}
