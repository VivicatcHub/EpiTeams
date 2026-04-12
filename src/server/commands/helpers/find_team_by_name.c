/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** find_team_by_name.c
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
