/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** save_users.c
*/

#include "serialization.h"

static void save_single_user(FILE *file, user_t *user)
{
    char uuid_str[UUID_STR_LEN];

    uuid_to_string(user->uuid, uuid_str);
    fprintf(file, "\t{\n\t\t\"uuid\":\"%s\","
        "\n\t\t\"name\":\"%s\""
        "\n\t}",
        uuid_str, user->name);
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
