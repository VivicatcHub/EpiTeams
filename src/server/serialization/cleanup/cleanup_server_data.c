/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cleanup_server_data.c
*/

#include "serialization.h"

void cleanup_server_data(server_data_t *data)
{
    cleanup_users(&data->users);
    cleanup_teams(&data->teams);
    cleanup_channels(&data->channels);
    cleanup_threads(&data->threads);
    cleanup_comments(&data->comments);
    cleanup_messages(&data->messages);
}
