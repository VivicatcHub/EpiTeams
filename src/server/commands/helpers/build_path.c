/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** build_path.c
*/

#include "my_teams.h"

static void handle_absolute_path(char const *args, char const *root, char *tmp)
{
    snprintf(tmp, PATH_MAX, "%s%s", root, args);
}

static void handle_relative_path(char const *args, char const *dir, char *tmp)
{
    snprintf(tmp, PATH_MAX, "%s/%s", dir, args);
}

static int resolve_existing_path(client_t *client, char const *tmp, char *out)
{
    if (realpath(tmp, out))
        return path_within_root(client, out);
    return 0;
}

static int resolve_path(client_t *client, char const *tmp, char *out,
    size_t sz)
{
    char dir_part[PATH_MAX];
    char base_part[PATH_MAX];
    char resolved_dir[PATH_MAX];
    char const *slash;

    slash = strrchr(tmp, '/');
    if (!slash)
        return 0;
    strncpy(dir_part, tmp, (size_t)(slash - tmp));
    dir_part[slash - tmp] = '\0';
    strncpy(base_part, slash + 1, PATH_MAX - 1);
    base_part[PATH_MAX - 1] = '\0';
    if (!realpath(dir_part, resolved_dir))
        return 0;
    if (!path_within_root(client, resolved_dir))
        return 0;
    snprintf(out, sz, "%s/%s", resolved_dir, base_part);
    return 1;
}

int build_path(client_t *client, char const *args, char *out, size_t sz)
{
    char tmp[PATH_MAX];
    char const *root = ((server_t *)client->server)->root_path;

    if (!args || args[0] == '\0')
        return 0;
    if (args[0] == '/')
        handle_absolute_path(args, root, tmp);
    else
        handle_relative_path(args, client->current_dir, tmp);
    if (resolve_existing_path(client, tmp, out))
        return 1;
    return resolve_path(client, tmp, out, sz);
}
