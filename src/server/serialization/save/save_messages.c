/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** save_messages.c
*/

#include "serialization.h"

static void save_single_message(FILE *file, private_message_t *msg)
{
    char uuid_str[UUID_STR_LEN];
    char sender_uuid_str[UUID_STR_LEN];
    char receiver_uuid_str[UUID_STR_LEN];

    uuid_to_string(msg->uuid, uuid_str);
    uuid_to_string(msg->sender_uuid, sender_uuid_str);
    uuid_to_string(msg->receiver_uuid, receiver_uuid_str);
    fprintf(file, "{\"uuid\":\"%s\",\"sender_uuid\":\"%s\","
        "\"receiver_uuid\":\"%s\",\"body\":\"%s\",\"created_at\":%ld}",
        uuid_str, sender_uuid_str, receiver_uuid_str, msg->body,
        msg->created_at);
}

int save_messages(struct message_queue_s *messages)
{
    FILE *file;
    private_message_t *msg;

    file = open_save_file(MESSAGES_FILE);
    if (!file)
        return -1;
    fprintf(file, "[");
    msg = TAILQ_FIRST(messages);
    while (msg) {
        save_single_message(file, msg);
        msg = TAILQ_NEXT(msg, entries);
        if (msg)
            fprintf(file, ",");
    }
    fprintf(file, "]\n");
    fclose(file);
    return 0;
}
