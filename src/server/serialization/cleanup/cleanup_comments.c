/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** cleanup_comments.c
*/

#include "serialization.h"

void cleanup_comments(struct comment_queue_s *comments)
{
    comment_t *comment;

    while (!TAILQ_EMPTY(comments)) {
        comment = TAILQ_FIRST(comments);
        TAILQ_REMOVE(comments, comment, entries);
        free(comment);
    }
}
