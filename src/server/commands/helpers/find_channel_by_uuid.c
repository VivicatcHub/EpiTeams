/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** find_channel_by_uuid.c
*/

#include "my_teams.h"

channel_t *find_channel_by_uuid(server_t *server, uuid_t uuid)
{
    channel_t *channel = NULL;

    TAILQ_FOREACH(channel, &server->data.channels, entries) {
        if (uuid_compare(channel->uuid, uuid) == 0)
            return channel;
    }
    return NULL;
}
