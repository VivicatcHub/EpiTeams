/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** my_teams.h
*/

#ifndef MY_TEAMS_H
    #define MY_TEAMS_H

    #include <string.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <unistd.h>

    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <sys/stat.h>
    #include <sys/wait.h>
    #include <signal.h>
    #include <fcntl.h>
    #include <poll.h>
    #include <errno.h>
    #include <dirent.h>
    #include <time.h>

    #include "server.h"
    #include "client.h"
    #include "protocol.h"
    #include "commands.h"

    #define UNUSED __attribute__((unused))

    #define MAX_NAME_LENGTH 32
    #define MAX_DESCRIPTION_LENGTH 255
    #define MAX_BODY_LENGTH 512

    #define EXIT_ERROR 84

typedef struct args_parser_s {
    int h1;
    int h2;
    int h3;
    int h4;
    int p1;
    int p2;
} args_parser_t;

typedef struct help_entries_s {
    char const *name;
    char const *description;
} help_entries_t;

char *get_ip_address(struct in_addr *addr);
int get_port(in_port_t *port);
#endif
