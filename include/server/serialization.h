/*
** EPITECH PROJECT, 2026
** my_teams
** File description:
** serialization.h
*/

#ifndef SERIALIZATION_H
    #define SERIALIZATION_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <uuid/uuid.h>

    #include "data_structures.h"

    #define SAVE_DIR "./data"
    #define USERS_FILE SAVE_DIR"/users.json"
    #define TEAMS_FILE SAVE_DIR"/teams.json"
    #define CHANNELS_FILE SAVE_DIR"/channels.json"
    #define THREADS_FILE SAVE_DIR"/threads.json"
    #define COMMENTS_FILE SAVE_DIR"/comments.json"
    #define MESSAGES_FILE SAVE_DIR"/messages.json"

int load_server_data(server_data_t *data);
int load_users(struct user_queue_s *users);
int load_teams(struct team_queue_s *teams);
int load_channels(struct channel_queue_s *channels);
int load_threads(struct thread_queue_s *threads);
int load_comments(struct comment_queue_s *comments);
int load_messages(struct message_queue_s *messages);

int save_server_data(server_data_t *data);
int save_users(struct user_queue_s *users);
int save_teams(struct team_queue_s *teams);
int save_channels(struct channel_queue_s *channels);
int save_threads(struct thread_queue_s *threads);
int save_comments(struct comment_queue_s *comments);
int save_messages(struct message_queue_s *messages);

void cleanup_server_data(server_data_t *data);
void cleanup_users(struct user_queue_s *users);
void cleanup_teams(struct team_queue_s *teams);
void cleanup_channels(struct channel_queue_s *channels);
void cleanup_threads(struct thread_queue_s *threads);
void cleanup_comments(struct comment_queue_s *comments);
void cleanup_messages(struct message_queue_s *messages);

void uuid_to_string(uuid_t uuid, char *str);
int string_to_uuid(char const *str, uuid_t uuid);
FILE *open_save_file(char const *filepath);
FILE *open_load_file(char const *filepath);
int save_dir_exists(void);

#endif
