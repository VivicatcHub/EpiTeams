/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** do_list.c
*/

#include "my_teams.h"

static void fmt_mode(mode_t mode, char *buf)
{
    if (S_ISDIR(mode))
        buf[0] = 'd';
    else if (S_ISLNK(mode))
        buf[0] = 'l';
    else
        buf[0] = '-';
    buf[1] = (mode & S_IRUSR) ? 'r' : '-';
    buf[2] = (mode & S_IWUSR) ? 'w' : '-';
    buf[3] = (mode & S_IXUSR) ? 'x' : '-';
    buf[4] = (mode & S_IRGRP) ? 'r' : '-';
    buf[5] = (mode & S_IWGRP) ? 'w' : '-';
    buf[6] = (mode & S_IXGRP) ? 'x' : '-';
    buf[7] = (mode & S_IROTH) ? 'r' : '-';
    buf[8] = (mode & S_IWOTH) ? 'w' : '-';
    buf[9] = (mode & S_IXOTH) ? 'x' : '-';
    buf[10] = '\0';
}

static void fmt_time(time_t mtime, char *buf)
{
    struct tm *tm_info = localtime(&mtime);

    strftime(buf, 16, "%b %d %H:%M", tm_info);
}

static void write_entry(int data_fd, char const *path, struct dirent *entry)
{
    struct stat st;
    char fpath[PATH_MAX];
    char modestr[11];
    char tmbuf[16];
    char buf[PATH_MAX + 128];
    int len;

    snprintf(fpath, sizeof(fpath), "%s/%s", path, entry->d_name);
    if (lstat(fpath, &st) < 0)
        return;
    fmt_mode(st.st_mode, modestr);
    fmt_time(st.st_mtime, tmbuf);
    len = snprintf(buf, sizeof(buf),
        "%s %3lu root root %8lld %s %s\r\n",
        modestr,
        (unsigned long)st.st_nlink,
        (long long)st.st_size,
        tmbuf,
        entry->d_name);
    write(data_fd, buf, len);
}

void list_command(int data_fd, char const *path)
{
    DIR *dir = opendir(path);
    struct dirent *entry;

    if (!dir)
        return;
    entry = readdir(dir);
    while (entry != NULL) {
        write_entry(data_fd, path, entry);
        entry = readdir(dir);
    }
    closedir(dir);
}
