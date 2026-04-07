/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** utils.c
*/

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include "my_teams.h"
#include "serialization.h"

void uuid_to_string(uuid_t uuid, char *str)
{
    uuid_unparse(uuid, str);
}

void string_to_uuid(char const *str, uuid_t uuid)
{
    uuid_parse(str, uuid);
}

int save_dir_exists(void)
{
    struct stat st;

    if (stat(SAVE_DIR, &st) == 0) {
        if (!S_ISDIR(st.st_mode)) {
            printf("Error: %s exists but is not a directory\n", SAVE_DIR);
            exit(EXIT_ERROR);
        }
    }
    if (errno == ENOENT) {
        mkdir(SAVE_DIR, 0755);
        return 1;
    }
}

FILE *open_save_file(char const *filepath)
{
    return fopen(filepath, "w");
}

FILE *open_load_file(char const *filepath)
{
    return fopen(filepath, "r");
}
