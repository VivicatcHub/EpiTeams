/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** find_team_by_uuid.c
*/

#include "my_teams.h"

team_t *find_team_by_name(server_t *server, char const *name)
{
    team_t *team = NULL;

    TAILQ_FOREACH(team, &server->data.teams, entries) {
        if (strcmp(team->name, name) == 0)
            return team;
    }
    return NULL;
}

team_t *find_team_by_uuid(server_t *server, uuid_t uuid)
{
    team_t *team = NULL;

    TAILQ_FOREACH(team, &server->data.teams, entries) {
        if (uuid_compare(team->uuid, uuid) == 0)
            return team;
    }
    return NULL;
}
