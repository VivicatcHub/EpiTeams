/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cleanup_channels.c
*/

#include "serialization.h"

void cleanup_channels(struct channel_queue_s *channels)
{
    channel_t *channel;
    thread_t *thread;

    while (!TAILQ_EMPTY(channels)) {
        channel = TAILQ_FIRST(channels);
        TAILQ_REMOVE(channels, channel, entries);
        while (!TAILQ_EMPTY(&channel->threads)) {
            thread = TAILQ_FIRST(&channel->threads);
            TAILQ_REMOVE(&channel->threads, thread, entries);
            free(thread);
        }
        free(channel);
    }
}
