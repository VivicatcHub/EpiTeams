#pragma once

#include <string>
#include <vector>

class MessageParser {
public:
    static bool parseUserStatus(const std::string &message, std::string &user_name, std::string &user_uuid, int &user_status);
    static bool parseTeamInfo(const std::string &message, std::string &team_name, std::string &team_uuid, std::string &team_description);
    static bool parseChannelInfo(const std::string &message, std::string &channel_name, std::string &channel_uuid, std::string &channel_description);
    static bool parseThreadInfo(const std::string &message, std::string &thread_uuid, std::string &user_uuid, long &timestamp, std::string &thread_title, std::string &thread_body);
    static bool parsePrivateMessage(const std::string &message, std::string &sender_uuid, long &timestamp, std::string &message_body);
    static bool parseThreadReplyInfo(const std::string &message, std::string &thread_uuid, std::string &user_uuid, long &timestamp, std::string &reply_body);
    static bool parseThreadCreationReceipt(const std::string &message, std::string &thread_uuid, std::string &user_uuid, time_t &thread_timestamp, std::string &thread_title, std::string &thread_body);
    static bool parseThreadReplyCreationReceipt(const std::string &message, std::string &team_uuid, std::string &thread_uuid, std::string &user_uuid, std::string &reply_body);
    static bool parseTeamCreationReceipt(const std::string &message, std::string &team_uuid, std::string &team_name, std::string &team_description);
    static bool parseChannelCreationReceipt(const std::string &message, std::string &channel_uuid, std::string &channel_name, std::string &channel_description);
    static bool parseLoginSuccess(const std::string &message, std::string &user_name, std::string &user_uuid);
    static bool parseLogoutSuccess(const std::string &message, std::string &user_name, std::string &user_uuid);
    static bool parseMessageReceipt(const std::string &message, std::string &msg, std::string &from);
    static bool parseSubscribeResponse(const std::string &message, std::string &team_uuid, std::string &user_uuid);
    static bool parseUnsubscribeResponse(const std::string &message, std::string &team_uuid, std::string &user_uuid);
    static std::string extractUuid(const std::string &message);
    static std::string cleanString(const std::string &value);
    static void printResponsePrompt(const std::string &code);
    static void printResponseFailure(const std::string &message);

private:
    MessageParser() = delete;
    ~MessageParser() = delete;
};
