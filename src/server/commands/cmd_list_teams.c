/*
** EPITECH PROJECT, 2026
** G-NWP-400-LYN-4-1-myteams-1
** File description:
** cmd_list_team
*/

#include "my_teams.h"

static int ensure_logged_in(server_t *server, client_t *client)
{
    if (!client || !client->username ||
        !find_user_by_name(server, client->username)) {
        send_response(client, CODE_NOT_LOGGED_IN, "Not logged in.");
        return 0;
    }
    return 1;
}

void cmd_list_teams2(client_t *client, team_t *team)
{
    char uuid[UUID_STR_LEN];
    char buf[MAX_BUFFER];

    uuid_to_string(team->uuid, uuid);
    snprintf(buf, MAX_BUFFER, "%s (%s) - %s", team->name, uuid,
        team->description);
    send_response(client, CODE_COMMAND_BEGIN, buf);
}

void cmd_list_teams(server_t *server, client_t *client, char *args UNUSED)
{
    if (ensure_logged_in(server, client) == 0)
        return;
    for (team_t *team = server->data.teams.tqh_first; team != NULL; team =
        team->entries.tqe_next) {
        cmd_list_teams2(client, team);
    }
    send_response(client, CODE_COMMAND_END, "No more teams.");
}
