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
    strncpy(name, server->data.users.tqh_first->name, MAX_NAME_LENGTH);
    //how to find their name.. hmh. Maybe
    // On récup depuis la séance
    // Pour afficher le reste : on fait une boucle qui parcours jusqu'à son name et on affiche tout
    send_response(client, CODE_COMMAND_OK, name);
    send_response(client, CODE_COMMAND_OK, "hhahhauabdibubozebdôub");
    // cmd_placeholder(server, client, args);
}

// fuser -k 4242/tcp
// make test_server
// make test_client