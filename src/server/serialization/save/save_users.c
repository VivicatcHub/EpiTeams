/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** save_users.c
*/

#include "serialization.h"

int save_users(struct user_queue_s *users)
{
    FILE *file;
    user_t *user;
    char uuid_str[UUID_STR_LEN];

    file = open_save_file(USERS_FILE);
    if (!file)
        return -1;
    fprintf(file, "[");
    user = TAILQ_FIRST(users);
    while (user) {
        uuid_to_string(user->uuid, uuid_str);
        fprintf(file, "{\"uuid\":\"%s\",\"name\":\"%s\",\"status\":%d}",
            uuid_str, user->name, user->status);
        user = TAILQ_NEXT(user, entries);
        if (user)
            fprintf(file, ",");
    }
    fprintf(file, "]\n");
    fclose(file);
    return 0;
}
