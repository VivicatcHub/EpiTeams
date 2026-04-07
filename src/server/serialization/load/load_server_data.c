/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** load_server_data.c
*/

#include "serialization.h"

int load_server_data(server_data_t *data)
{
    if (save_dir_exists() == 1)
        save_server_data(data);
    if (load_users(&data->users) == -1)
        return -1;
    if (load_teams(&data->teams) == -1)
        return -1;
    if (load_channels(&data->channels) == -1)
        return -1;
    if (load_threads(&data->threads) == -1)
        return -1;
    if (load_comments(&data->comments) == -1)
        return -1;
    if (load_messages(&data->messages) == -1)
        return -1;
    return 0;
}
