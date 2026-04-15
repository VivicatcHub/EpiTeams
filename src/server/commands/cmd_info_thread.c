/*
** EPITECH PROJECT, 2026
** G-NWP-400-LYN-4-1-myteams-1
** File description:
** cmd_info_thread
*/

#include "my_teams.h"

void cmd_info_thread(server_t *server, client_t *client, char *args)
{
    int exist = 0;
    char result[MAX_BUFFER];
    char uuid_buf[UUID_STR_LEN];

    for (channel_t *thread = server->data.threads.tqh_first; thread != NULL;
        thread = thread->entries.tqe_next) {
        if (strcmp(args, thread->name) == 0) {
            uuid_to_string(thread->uuid, uuid_buf);
            snprintf(result, sizeof(result), "thread name : %s | Uuid : %s\n",
                thread->name, uuid_buf);
            send_response(client, CODE_COMMAND_OK, result);
            exist = 1;
        }
    }
    if (exist == 0) {
        snprintf(result, sizeof(result), "Thread name : %s doesn't exist", args);
        send_response(client, CODE_NOT_FOUND, result);
    }
}
