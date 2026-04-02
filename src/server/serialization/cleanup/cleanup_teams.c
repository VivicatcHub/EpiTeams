/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cleanup_teams.c
*/

#include "serialization.h"

void cleanup_teams(struct team_queue_s *teams)
{
    team_t *team;
    team_member_t *member;
    channel_t *channel;

    while (!TAILQ_EMPTY(teams)) {
        team = TAILQ_FIRST(teams);
        TAILQ_REMOVE(teams, team, entries);
        while (!TAILQ_EMPTY(&team->members)) {
            member = TAILQ_FIRST(&team->members);
            TAILQ_REMOVE(&team->members, member, entries);
            free(member);
        }
        while (!TAILQ_EMPTY(&team->channels)) {
            channel = TAILQ_FIRST(&team->channels);
            TAILQ_REMOVE(&team->channels, channel, entries);
            free(channel);
        }
        free(team);
    }
}
