/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** data_structures.h
*/

#ifndef DATA_STRUCTURES_H
    #define DATA_STRUCTURES_H

    #include <sys/queue.h>
    #include <uuid/uuid.h>
    #include <time.h>

    #define MAX_NAME_LENGTH 32
    #define MAX_DESCRIPTION_LENGTH 255
    #define MAX_BODY_LENGTH 512
    #define UUID_STR_LEN 37

typedef enum user_status_e {
    OFFLINE,
    ONLINE
} user_status_t;

typedef struct user_s {
    uuid_t uuid;
    char name[MAX_NAME_LENGTH];
    user_status_t status;
    TAILQ_HEAD(, team_subscription_s) subscribed_teams;
    TAILQ_ENTRY(user_s) entries;
} user_t;

typedef struct team_subscription_s {
    uuid_t team_uuid;
    TAILQ_ENTRY(team_subscription_s) entries;
} team_subscription_t;

TAILQ_HEAD(user_queue_s, user_s);

typedef struct team_s {
    uuid_t uuid;
    char name[MAX_NAME_LENGTH];
    char description[MAX_DESCRIPTION_LENGTH];
    TAILQ_HEAD(, team_member_s) members;
    TAILQ_HEAD(, channel_s) channels;
    time_t created_at;
    TAILQ_ENTRY(team_s) entries;
} team_t;

typedef struct team_member_s {
    uuid_t user_uuid;
    TAILQ_ENTRY(team_member_s) entries;
} team_member_t;

TAILQ_HEAD(team_queue_s, team_s);

typedef struct channel_s {
    uuid_t uuid;
    uuid_t team_uuid;
    char name[MAX_NAME_LENGTH];
    char description[MAX_DESCRIPTION_LENGTH];
    TAILQ_HEAD(, thread_s) threads;
    time_t created_at;
    TAILQ_ENTRY(channel_s) entries;
} channel_t;

TAILQ_HEAD(channel_queue_s, channel_s);

typedef struct thread_s {
    uuid_t uuid;
    uuid_t channel_uuid;
    uuid_t team_uuid;
    uuid_t author_uuid;
    char title[MAX_NAME_LENGTH];
    char message[MAX_BODY_LENGTH];
    time_t created_at;
    TAILQ_HEAD(, comment_s) comments;
    TAILQ_ENTRY(thread_s) entries;
} thread_t;

TAILQ_HEAD(thread_queue_s, thread_s);

typedef struct comment_s {
    uuid_t uuid;
    uuid_t thread_uuid;
    uuid_t author_uuid;
    char body[MAX_BODY_LENGTH];
    time_t created_at;
    TAILQ_ENTRY(comment_s) entries;
} comment_t;

TAILQ_HEAD(comment_queue_s, comment_s);

typedef struct private_message_s {
    uuid_t uuid;
    uuid_t sender_uuid;
    uuid_t receiver_uuid;
    char body[MAX_BODY_LENGTH];
    time_t created_at;
    TAILQ_ENTRY(private_message_s) entries;
} private_message_t;

TAILQ_HEAD(message_queue_s, private_message_s);

typedef struct server_data_s {
    struct user_queue_s users;
    struct team_queue_s teams;
    struct channel_queue_s channels;
    struct thread_queue_s threads;
    struct comment_queue_s comments;
    struct message_queue_s messages;
} server_data_t;
#endif
