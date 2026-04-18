/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** load_users.c
*/

#include "my_teams.h"

static int has_next_comma(FILE *file)
{
    int c = fgetc(file);

    while (c == ' ' || c == '\n' || c == '\r' || c == '\t')
        c = fgetc(file);
    if (c != ',') {
        ungetc(c, file);
        return 0;
    }
    ungetc(c, file);
    return 1;
}

static int parse_subscribed_teams(FILE *file, user_t *user)
{
    char team_uuid_str[UUID_STR_LEN];
    team_subscription_t *sub;

    fscanf(file, ",\n\t\t\"subscribed_teams\":\n\t\t[");
    while (fscanf(file, "\n\t\t\t{\n\t\t\t\t\"team_uuid"
            "\":\"%36[^\"]\"\n\t\t\t}",
            team_uuid_str) == 1) {
        sub = malloc(sizeof(team_subscription_t));
        if (!sub)
            return -1;
        string_to_uuid(team_uuid_str, sub->team_uuid);
        TAILQ_INSERT_TAIL(&user->subscribed_teams, sub, entries);
        fscanf(file, ",");
    }
    fscanf(file, "\n\t\t]");
    return 0;
}

static int load_single_user(FILE *file, char *uuid_str, user_t *user)
{
    string_to_uuid(uuid_str, user->uuid);
    if (fscanf(file, ",\n\t\t\"name\":\"%31[^\"]\"", user->name) != 1)
        return -1;
    TAILQ_INIT(&user->subscribed_teams);
    if (has_next_comma(file)) {
        if (parse_subscribed_teams(file, user) == -1)
            return -1;
    }
    fscanf(file, "\n\t}");
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
        return -1;
    printf("Loading users...\n");
    fscanf(file, "[");
    while (fscanf(file, "\n\t{\n\t\t\"uuid\":\"%36[^\"]\"", uuid_str) == 1) {
        if (append_user(users, file, uuid_str) == -1) {
            fclose(file);
            return -1;
        }
        fscanf(file, ",");
    }
    fclose(file);
    return 0;
}
