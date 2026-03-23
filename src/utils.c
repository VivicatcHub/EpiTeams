/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** utils.c
*/

#include "my_teams.h"

char *get_ip_address(struct in_addr *addr)
{
    return inet_ntoa(*addr);
}

int get_port(in_port_t *port)
{
    return ntohs(*port);
}
