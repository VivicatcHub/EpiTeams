/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** save_users.c
*/

#include "my_teams.h"

static void save_single_subscription(FILE *file, team_subscription_t *sub)
{
    char team_uuid_str[UUID_STR_LEN];

    uuid_to_string(sub->team_uuid, team_uuid_str);
    fprintf(file, "\n\t\t\t{\n\t\t\t\t\"team_uuid\":\"%s\"\n\t\t\t}",
        team_uuid_str);
}

static void save_single_user_subscriptions(FILE *file, user_t *user)
{
    team_subscription_t *sub = NULL;

    fprintf(file, ",\n\t\t\"subscribed_teams\":\n\t\t[");
    TAILQ_FOREACH(sub, &user->subscribed_teams, entries) {
        save_single_subscription(file, sub);
        if (TAILQ_NEXT(sub, entries))
            fprintf(file, ",");
    }
    fprintf(file, "\n\t\t]");
}

static void save_single_user(FILE *file, user_t *user)
{
    char uuid_str[UUID_STR_LEN];

    uuid_to_string(user->uuid, uuid_str);
    fprintf(file, "\t{\n\t\t\"uuid\":\"%s\","
        "\n\t\t\"name\":\"%s\"",
        uuid_str, user->name);
    if (!TAILQ_EMPTY(&user->subscribed_teams))
        save_single_user_subscriptions(file, user);
    fprintf(file, "\n\t}");
}

int save_users(struct user_queue_s *users)
{
    FILE *file;
    user_t *user;

    file = open_save_file(USERS_FILE);
    if (!file)
        return -1;
    fprintf(file, "[");
    user = TAILQ_FIRST(users);
    if (user)
        fprintf(file, "\n");
    while (user) {
        save_single_user(file, user);
        user = TAILQ_NEXT(user, entries);
        if (user)
            fprintf(file, ",\n");
    }
    fprintf(file, "\n]\n");
    fclose(file);
    return 0;
}
