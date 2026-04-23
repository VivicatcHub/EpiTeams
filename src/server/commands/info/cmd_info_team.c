/*
** EPITECH PROJECT, 2026
** G-NWP-400-LYN-4-1-myteams-1
** File description:
** cmd_info_team
*/

#include "my_teams.h"
#include <time.h>

void cmd_info_team_2(client_t *client, team_t *team_ok)
{
    char uuid_buf[UUID_STR_LEN];
    char result[MAX_BUFFER];
    char time_buf[64];
    time_t t = (time_t)team_ok->created_at;
    struct tm tm_info;

    uuid_to_string(team_ok->uuid, uuid_buf);
    localtime_r(&t, &tm_info);
    strftime(time_buf, sizeof(time_buf), "%d/%m/%Y %H:%M:%S", &tm_info);
    snprintf(result, sizeof(result),
        "Team Name : %s | Uuid : %s\n Created at %s | Description : %s",
        team_ok->name, uuid_buf, time_buf, team_ok->description);
    send_response(client, CODE_COMMAND_OK, result);
}

void cmd_info_team(server_t *server, client_t *client, char *args)
{
    user_t *user = find_user_by_name(server, client->username);
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
    if (require_user_subscribed_to_team(server, client, user, team_ok) != 0)
        return;
    cmd_info_team_2(client, team_ok);
}
