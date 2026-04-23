/*
** EPITECH PROJECT, 2026
** MyTeams
** File description:
** search_types_2.c
*/

#include "my_teams.h"

int require_user_subscribed_to_team(server_t *server UNUSED, client_t *client,
    user_t *user, team_t *team)
{
    if (!user || !team) {
        send_response(client, CODE_NOT_LOGGED_IN, "Not logged in.");
        return -1;
    }
    if (!is_user_subscribed_to_team(user, team->uuid)) {
        send_response(client, CODE_NOT_AUTHORIZED,
            "You are not subscribed to this team.");
        return -1;
    }
    return 0;
}
