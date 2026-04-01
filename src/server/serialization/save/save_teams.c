/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** save_teams.c
*/

#include "serialization.h"

static void save_single_team(FILE *file, team_t *team)
{
    char uuid_str[UUID_STR_LEN];

    uuid_to_string(team->uuid, uuid_str);
    fprintf(file, "{\"uuid\":\"%s\",\"name\":\"%s\",\"description\":\"%s\","
        "\"created_at\":%ld}",
        uuid_str, team->name, team->description, team->created_at);
}

int save_teams(struct team_queue_s *teams)
{
    FILE *file;
    team_t *team;

    file = open_save_file(TEAMS_FILE);
    if (!file)
        return -1;
    fprintf(file, "[");
    team = TAILQ_FIRST(teams);
    while (team) {
        save_single_team(file, team);
        team = TAILQ_NEXT(team, entries);
        if (team)
            fprintf(file, ",");
    }
    fprintf(file, "]\n");
    fclose(file);
    return 0;
}
