/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** load_teams.c
*/

#include "serialization.h"

static int load_single_team(FILE *file, char *uuid_str, team_t *team)
{
    long created_at;

    string_to_uuid(uuid_str, team->uuid);
    fscanf(file, ",\"name\":\"%31[^\"]\"", team->name);
    fscanf(file, ",\"description\":\"%254[^\"]\"", team->description);
    fscanf(file, ",\"created_at\":%ld", &created_at);
    team->created_at = (time_t)created_at;
    TAILQ_INIT(&team->members);
    TAILQ_INIT(&team->channels);
    fscanf(file, "}");
    return 0;
}

static int append_team(struct team_queue_s *teams, FILE *file, char *uuid_str)
{
    team_t *team;

    team = malloc(sizeof(team_t));
    if (!team)
        return -1;
    if (load_single_team(file, uuid_str, team) == -1) {
        free(team);
        return -1;
    }
    TAILQ_INSERT_TAIL(teams, team, entries);
    return 0;
}

int load_teams(struct team_queue_s *teams)
{
    FILE *file;
    char uuid_str[UUID_STR_LEN];

    TAILQ_INIT(teams);
    file = open_load_file(TEAMS_FILE);
    if (!file)
        return 0;
    while (fscanf(file, "{\"uuid\":\"%36[^\"]\"", uuid_str) == 1) {
        if (append_team(teams, file, uuid_str) == -1) {
            fclose(file);
            return -1;
        }
    }
    fclose(file);
    return 0;
}
