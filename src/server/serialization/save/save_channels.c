/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** save_channels.c
*/

#include "serialization.h"

static void save_single_channel(FILE *file, channel_t *channel)
{
    char uuid_str[UUID_STR_LEN];
    char team_uuid_str[UUID_STR_LEN];

    uuid_to_string(channel->uuid, uuid_str);
    uuid_to_string(channel->team_uuid, team_uuid_str);
    fprintf(file, "{\"uuid\":\"%s\",\"team_uuid\":\"%s\",\"name\":\"%s\","
        "\"description\":\"%s\",\"created_at\":%ld}",
        uuid_str, team_uuid_str, channel->name, channel->description,
        channel->created_at);
}

int save_channels(struct channel_queue_s *channels)
{
    FILE *file;
    channel_t *channel;

    file = open_save_file(CHANNELS_FILE);
    if (!file)
        return -1;
    fprintf(file, "[");
    channel = TAILQ_FIRST(channels);
    while (channel) {
        save_single_channel(file, channel);
        channel = TAILQ_NEXT(channel, entries);
        if (channel)
            fprintf(file, ",");
    }
    fprintf(file, "]\n");
    fclose(file);
    return 0;
}
