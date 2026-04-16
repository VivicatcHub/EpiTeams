/*
** EPITECH PROJECT, 2026
** G-NWP-400-LYN-4-1-myteams-1
** File description:
** cmd_info_team
*/

#include "my_teams.h"

void cmd_info_team(server_t *server, client_t *client, char *args)
{
    int exist = 0;
    char result[MAX_BUFFER];
    char uuid_buf[UUID_STR_LEN];
    char *team_uuid[MAX_BUFFER];
    int64_t time_;

    team_t *team;
    team_t *team_;

    team = find_team_by_uuid(server, args);
    team_ = find_team_by_name(server, args);
    if (team == NULL && team_ == NULL)
        return;

    if (team != NULL) {
        uuid_to_string(team->uuid, uuid_buf);
        snprintf(result, sizeof(result), "Team name : %s | Uuid : %s\n Created at %i | Description : %s",
            team->name, uuid_buf, time_, team->description);
        send_response(client, CODE_COMMAND_OK, result);
        exist = 1;
    }
    if (team_ != NULL) {
        uuid_to_string(team_->uuid, uuid_buf);
        snprintf(result, sizeof(result), "Team name : %s | Uuid : %s\n Created at %i | Description : %s",
            team_->name, uuid_buf, time_, team_->description);
        send_response(client, CODE_COMMAND_OK, result);
        exist = 1;
    }
    if (exist == 0) {
        snprintf(result, sizeof(result), "Team name : %s doesn't exist",
            args);
        send_response(client, CODE_NOT_FOUND, result);
    }
}

