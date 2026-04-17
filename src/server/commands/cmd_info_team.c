/*
** EPITECH PROJECT, 2026
** G-NWP-400-LYN-4-1-myteams-1
** File description:
** cmd_info_team
*/

#include "my_teams.h"

void cmd_info_team(server_t *server, client_t *client, char *args)
{
    char result[MAX_BUFFER];
    char uuid_buf[UUID_STR_LEN];
    int64_t time_;
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
    time_ = (int64_t)team_ok->created_at;
    uuid_to_string(team_ok->uuid, uuid_buf);
    snprintf(result, sizeof(result), "Team Name : %s | Uuid : %s\n Created at %i | Description : %s",
        team_ok->name, uuid_buf, time_, team_ok->description);
    send_response(client, CODE_COMMAND_OK, result);
}



    // string_to_uuid(args, uuid);
    // team = find_team_by_uuid(server, uuid);
    // if (!team)
    //     return;
    // snprintf(result, sizeof(result), "Found team : [%s]", team->name);
    // send_response(client, CODE_NOT_FOUND, result);

    //find team works.
    // team_ = find_team_by_name(server, args);
    // if (!team_)
    //     return;
    // uuid_to_string(team_->uuid, uuid_buf);
    // snprintf(result, sizeof(result), "team uuid [%s]", uuid_buf);
    // send_response(client, CODE_COMMAND_OK, result);


    // string_to_uuid(args, uuid);
    // snprintf(result, sizeof(result), "Args : {%s}", args);
    // send_response(client, CODE_NOT_FOUND, result);
    // team = find_team_by_uuid(server, uuid); //est-ce qu'il arrive à détecté uuid ?
    // team_ = find_team_by_name(server, args);
    // if (!team)
    //     return;
    // send_response(client, CODE_NOT_FOUND, "Found team");
    // snprintf(result, sizeof(result), "UUID : %s", team->uuid);
    // if (team == NULL/*&& team_ == NULL*/) {
    //     snprintf(result, sizeof(result), "Team name : %s doesn't exist",
    //         args);
        // send_response(client, CODE_NOT_FOUND, result);
    //     return;
    // }
    // if (team != NULL) {
    //     send_response(client, CODE_COMMAND_OK, "coucou");
    //     time_ = (int64_t)team->created_at;
        // uuid_to_string(team->uuid, uuid_buf);
        // snprintf(result, sizeof(result), "Team name : %s | Uuid : %s\n Created at %i | Description : %s",
        //     team->name, uuid_buf, time_, team->description);
        // send_response(client, CODE_COMMAND_OK, result);
    //     exist = 1;
    // }
    // if (team_ != NULL) {
    //     send_response(client, CODE_COMMAND_OK, "coucouc 2");
    //     time_ = (int64_t)team_->created_at;
    //     uuid_to_string(team_->uuid, uuid_buf);
    //     snprintf(result, sizeof(result), "Team name : %s | Uuid : %s\n Created at %i | Description : %s",
    //         team_->name, uuid_buf, time_, team_->description);
    //     send_response(client, CODE_COMMAND_OK, result);
    //     exist = 1;
    // }