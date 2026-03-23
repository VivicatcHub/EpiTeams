/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** accept_client.c
*/

#include "my_teams.h"

void accept_client(server_t *server)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int fd;

    fd = accept(server->socket, (struct sockaddr *)&addr, &len);
    if (fd == -1) {
        perror("accept");
        return;
    }
    if (add_client(server, fd, &addr) == -1) {
        write(fd, "421 Too many connections.\r\n", 28);
        close(fd);
    }
}
