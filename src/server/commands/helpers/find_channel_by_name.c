/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** find_channel_by_name.c
*/

#include "my_teams.h"

channel_t *find_channel_by_name(server_t *server, char const *name)
{
    channel_t *channel = NULL;

    TAILQ_FOREACH(channel, &server->data.channels, entries) {
        if (strcmp(channel->name, name) == 0)
            return channel;
    }
    return NULL;
}
