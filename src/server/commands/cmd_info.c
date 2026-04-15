/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cmd_info.c
*/

#include "my_teams.h"

void cmd_info(server_t *server, client_t *client, char *args)
{
    char name[MAX_NAME_LENGTH];
    // send_response(client, CODE_COMMAND_OK, client->username);

    user_t *user = find_user_by_name(server, client->username);
    user->uuid;
    char uuid_buf[UUID_STR_LEN];
    uuid_to_string(user->uuid, uuid_buf);

    char result[MAX_BUFFER];
    if (ONLINE == user->status) {
        snprintf(result, sizeof(result), "User %s with uuid %s is connected", client->username, uuid_buf);
        send_response(client, CODE_COMMAND_OK, result);
    } else {
        snprintf(result, sizeof(result), "User %s with uuid %s is not connected", client->username, uuid_buf);
        send_response(client, CODE_COMMAND_OK, result);
    }
}

// fuser -k 4242/tcp
// make test_server
// make test_client



// note : j'ai pas mis de s à chaque fichier : "cmd_info_channels contrairement à val"
// (lors de la fonction handle)
// Et les strings qu'il renvoie, ne sont âs forcément les mêmes que ceux que je demande à récupérer.. attention.


// command handler : 
// bool CommandHandler::handleInfo() {
//     if (!context.isTeamContext()) { return sendCommand("INFO_USER"); }  <========== like this

//     if (!context.isChannelContext()) {
//         std::string cmd = "INFO_TEAM " + context.getTeamUuid();
//         return sendCommand(cmd);
//     }

//     if (!context.isThreadContext()) {
//         std::string cmd = "INFO_CHANNEL " + context.getChannelUuid();
//         return sendCommand(cmd);
//     }

//     std::string cmd = "INFO_THREAD " + context.getThreadUuid();
//     return sendCommand(cmd);
// }
