/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** find_user_by_uuid.c
*/

#include "my_teams.h"

user_t *find_user_by_uuid(server_t *server, uuid_t uuid)
{
    user_t *user = NULL;

    TAILQ_FOREACH(user, &server->data.users, entries) {
        if (uuid_compare(user->uuid, uuid) == 0)
            return user;
    }
    return NULL;
}
