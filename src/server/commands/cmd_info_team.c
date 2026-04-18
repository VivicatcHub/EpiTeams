/*
** EPITECH PROJECT, 2026
** G-NWP-400-LYN-4-1-myteams-1
** File description:
** cmd_info_team
*/

#include "my_teams.h"

void cmd_info_team_2(client_t *client, team_t *team_ok)
{
    char uuid_buf[UUID_STR_LEN];
    char result[MAX_BUFFER];

    uuid_to_string(team_ok->uuid, uuid_buf);
    snprintf(result, sizeof(result), "%s (%s) - %s", team_ok->name,
        uuid_buf, team_ok->description);
    send_response(client, CODE_COMMAND_BEGIN, result);
    send_response(client, CODE_COMMAND_END, "No more team info.");
}

void cmd_info_team(server_t *server, client_t *client, char *args)
{
    team_t *team;
    team_t *team_n;
    team_t *team_ok;
    uuid_t uuid;

    string_to_uuid(args, uuid);
    team = find_team_by_uuid(server, uuid);
    team_n = find_team_by_name(server, args);
    if (!team && !team_n) {
        send_response(client, CODE_NOT_FOUND, "Team not found.");
        return;
    }
    team_ok = (!team) ? team_n : team;
    cmd_info_team_2(client, team_ok);
}
