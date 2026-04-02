/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** save_server_data.c
*/

#include "serialization.h"

int save_server_data(server_data_t *data)
{
    save_dir_exists();
    if (save_users(&data->users) == -1)
        return -1;
    if (save_teams(&data->teams) == -1)
        return -1;
    if (save_channels(&data->channels) == -1)
        return -1;
    if (save_threads(&data->threads) == -1)
        return -1;
    if (save_comments(&data->comments) == -1)
        return -1;
    if (save_messages(&data->messages) == -1)
        return -1;
    return 0;
}
