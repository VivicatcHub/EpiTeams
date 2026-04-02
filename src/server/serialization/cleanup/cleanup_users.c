/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cleanup_users.c
*/

#include "serialization.h"

void cleanup_users(struct user_queue_s *users)
{
    user_t *user;
    team_subscription_t *sub;

    while (!TAILQ_EMPTY(users)) {
        user = TAILQ_FIRST(users);
        TAILQ_REMOVE(users, user, entries);
        while (!TAILQ_EMPTY(&user->subscribed_teams)) {
            sub = TAILQ_FIRST(&user->subscribed_teams);
            TAILQ_REMOVE(&user->subscribed_teams, sub, entries);
            free(sub);
        }
        free(user);
    }
}
