/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** load_messages.c
*/

#include "serialization.h"

static int load_single_message(FILE *file, char *uuid_str,
    private_message_t *msg)
{
    char sender_uuid_str[UUID_STR_LEN];
    char receiver_uuid_str[UUID_STR_LEN];
    long created_at;

    string_to_uuid(uuid_str, msg->uuid);
    fscanf(file, ",\n\t\t\"sender_uuid\":\"%36[^\"]\"", sender_uuid_str);
    string_to_uuid(sender_uuid_str, msg->sender_uuid);
    fscanf(file, ",\n\t\t\"receiver_uuid\":\"%36[^\"]\"", receiver_uuid_str);
    string_to_uuid(receiver_uuid_str, msg->receiver_uuid);
    fscanf(file, ",\n\t\t\"body\":\"%511[^\"]\"", msg->body);
    fscanf(file, ",\n\t\t\"created_at\":%ld", &created_at);
    msg->created_at = (time_t)created_at;
    fscanf(file, "\n\t}");
    return 0;
}

static int append_message(struct message_queue_s *messages, FILE *file,
    char *uuid_str)
{
    private_message_t *msg;

    msg = malloc(sizeof(private_message_t));
    if (!msg)
        return -1;
    if (load_single_message(file, uuid_str, msg) == -1) {
        free(msg);
        return -1;
    }
    TAILQ_INSERT_TAIL(messages, msg, entries);
    return 0;
}

int load_messages(struct message_queue_s *messages)
{
    FILE *file;
    char uuid_str[UUID_STR_LEN];

    TAILQ_INIT(messages);
    file = open_load_file(MESSAGES_FILE);
    if (!file)
        return -1;
    printf("Loading messages...\n");
    fscanf(file, "[");
    while (fscanf(file, "\n\t{\n\t\t\"uuid\":\"%36[^\"]\"", uuid_str) == 1) {
        if (append_message(messages, file, uuid_str) == -1) {
            fclose(file);
            return -1;
        }
        fscanf(file, ",");
    }
    fclose(file);
    return 0;
}
