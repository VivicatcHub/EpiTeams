/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** load_users.c
*/

#include "serialization.h"
#include "logging_server.h"

static int load_single_user(FILE *file, char *uuid_str, user_t *user)
{
    string_to_uuid(uuid_str, user->uuid);
    if (fscanf(file, ",\"name\":\"%31[^\"]\"", user->name) != 1)
        return -1;
    TAILQ_INIT(&user->subscribed_teams);
    fscanf(file, "}");
    return 0;
}

static int append_user(struct user_queue_s *users, FILE *file, char *uuid_str)
{
    user_t *user;
    char uuid_buf[UUID_STR_LEN];

    user = malloc(sizeof(user_t));
    if (!user)
        return -1;
    if (load_single_user(file, uuid_str, user) == -1) {
        free(user);
        return -1;
    }
    uuid_to_string(user->uuid, uuid_buf);
    user->status = OFFLINE;
    server_event_user_loaded(uuid_buf, user->name);
    TAILQ_INSERT_TAIL(users, user, entries);
    return 0;
}

int load_users(struct user_queue_s *users)
{
    FILE *file;
    char uuid_str[UUID_STR_LEN];

    TAILQ_INIT(users);
    file = open_load_file(USERS_FILE);
    if (!file)
        return 0;
    printf("Loading users...\n");
    fscanf(file, "[");
    while (fscanf(file, "{\"uuid\":\"%36[^\"]\"", uuid_str) == 1) {
        if (append_user(users, file, uuid_str) == -1) {
            fclose(file);
            return -1;
        }
        fscanf(file, ",");
    }
    fclose(file);
    return 0;
}
