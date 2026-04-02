/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** load_channels.c
*/

#include "serialization.h"

static int load_single_channel(FILE *file, char *uuid_str, channel_t *channel)
{
    char team_uuid_str[UUID_STR_LEN];
    long created_at;

    string_to_uuid(uuid_str, channel->uuid);
    fscanf(file, ",\"team_uuid\":\"%36[^\"]\"", team_uuid_str);
    string_to_uuid(team_uuid_str, channel->team_uuid);
    fscanf(file, ",\"name\":\"%31[^\"]\"", channel->name);
    fscanf(file, ",\"description\":\"%254[^\"]\"", channel->description);
    fscanf(file, ",\"created_at\":%ld", &created_at);
    channel->created_at = (time_t)created_at;
    TAILQ_INIT(&channel->threads);
    fscanf(file, "}");
    return 0;
}

static int append_channel(struct channel_queue_s *channels, FILE *file,
    char *uuid_str)
{
    channel_t *channel;

    channel = malloc(sizeof(channel_t));
    if (!channel)
        return -1;
    if (load_single_channel(file, uuid_str, channel) == -1) {
        free(channel);
        return -1;
    }
    TAILQ_INSERT_TAIL(channels, channel, entries);
    return 0;
}

int load_channels(struct channel_queue_s *channels)
{
    FILE *file;
    char uuid_str[UUID_STR_LEN];

    TAILQ_INIT(channels);
    file = open_load_file(CHANNELS_FILE);
    if (!file)
        return 0;
    while (fscanf(file, "{\"uuid\":\"%36[^\"]\"", uuid_str) == 1) {
        if (append_channel(channels, file, uuid_str) == -1) {
            fclose(file);
            return -1;
        }
    }
    fclose(file);
    return 0;
}
