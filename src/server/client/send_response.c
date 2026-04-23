/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** send_response.c
*/

#include "my_teams.h"

void send_response(client_t *client, char const *code, char const *msg)
{
    char buf[MAX_BUFFER];
    int len = snprintf(buf, sizeof(buf), "%s %s\n", code, msg);

    write(client->socket, buf, len);
    client->waiting_for_client_receipt = 1;
    if (strcmp(code, CODE_SERVICE_READY) == 0 || strcmp(code, CODE_QUIT) == 0)
        return;
    printf("Sent code %s to client %s:%d\n", code,
        get_ip_address(&client->address.sin_addr),
        get_port(&client->address.sin_port));
}
