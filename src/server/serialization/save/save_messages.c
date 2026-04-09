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
    fprintf(file, "\t{\n\t\t\"uuid\":\"%s\","
        "\n\t\t\"sender_uuid\":\"%s\","
        "\n\t\t\"receiver_uuid\":\"%s\","
        "\n\t\t\"body\":\"%s\","
        "\n\t\t\"created_at\":%ld"
        "\n\t}",
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
    if (msg)
        fprintf(file, "\n");
    while (msg) {
        save_single_message(file, msg);
        msg = TAILQ_NEXT(msg, entries);
        if (msg)
            fprintf(file, ",\n");
    }
    fprintf(file, "\n]\n");
    fclose(file);
    return 0;
}
