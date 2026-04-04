/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** find_user_by_name.c
*/

#include "my_teams.h"

user_t *find_user_by_name(server_t *server, char const *name)
{
    user_t *user = NULL;

    TAILQ_FOREACH(user, &server->data.users, entries) {
        if (strcmp(user->name, name) == 0)
            return user;
    }
    return NULL;
}
