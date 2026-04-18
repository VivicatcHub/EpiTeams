#include "class/ResponseHandler.hpp"
#include "class/Context.hpp"
#include "class/LoggingHandlerFactory.hpp"
#include "class/MessageParser.hpp"
#include "logging_client.h"
#include <cctype>
#include <iostream>

ResponseHandler::ResponseHandler() : _waiting_for_202(false) {}

void ResponseHandler::setCommandContext(const std::string &command, const std::vector<std::string> &args, const Context &context)
{
    _last_command = command;
    if (command == "create" && !context.isTeamContext() && !context.isChannelContext() && !context.isThreadContext()) _last_command = "create_team";
    else if (command == "create" && context.isTeamContext() && !context.isChannelContext() && !context.isThreadContext()) _last_command = "create_channel";
    else if (command == "create" && context.isTeamContext() && context.isChannelContext() && !context.isThreadContext()) _last_command = "create_thread";
    else if (command == "create" && context.isTeamContext() && context.isChannelContext() && context.isThreadContext()) _last_command = "create_comment";
    else if (command == "list" && !context.isTeamContext() && !context.isChannelContext() && !context.isThreadContext()) _last_command = "list_teams";
    else if (command == "list" && context.isTeamContext() && !context.isChannelContext() && !context.isThreadContext()) _last_command = "list_channels";
    else if (command == "list" && context.isTeamContext() && context.isChannelContext() && !context.isThreadContext()) _last_command = "list_threads";
    else if (command == "list" && context.isTeamContext() && context.isChannelContext() && context.isThreadContext()) _last_command = "list_comments";
    else if (command == "info" && !context.isTeamContext() && !context.isChannelContext() && !context.isThreadContext()) _last_command = "info_user";
    else if (command == "info" && context.isTeamContext() && !context.isChannelContext() && !context.isThreadContext()) _last_command = "info_team";
    else if (command == "info" && context.isTeamContext() && context.isChannelContext() && !context.isThreadContext()) _last_command = "info_channel";
    else if (command == "info" && context.isTeamContext() && context.isChannelContext() && context.isThreadContext()) _last_command = "info_thread";
    _last_args = args;
    _context = context;
}

std::string ResponseHandler::__cleanString(const std::string &value)
{
    size_t start = 0;
    size_t end = value.size();

    while (start < value.size() && std::isspace(static_cast<unsigned char>(value[start]))) start++;
    while (end > start && std::isspace(static_cast<unsigned char>(value[end - 1]))) end--;
    return value.substr(start, end - start);
}

bool ResponseHandler::__extractCodeAndMessage(const std::string &line, std::string &code, std::string &message)
{
    std::string cleaned = __cleanString(line);

    if (cleaned.empty()) return false;
    if (cleaned.size() < 3 || !std::isdigit(static_cast<unsigned char>(cleaned[0])) || !std::isdigit(static_cast<unsigned char>(cleaned[1])) || !std::isdigit(static_cast<unsigned char>(cleaned[2]))) {
        code = "---";
        message = cleaned;
        return true;
    }

    code = cleaned.substr(0, 3);
    if (cleaned.size() == 3) {
        message.clear();
        return true;
    }
    if (cleaned[3] == ' ') {
        message = cleaned.substr(4);
        return true;
    }
    message = cleaned.substr(3);
    return true;
}

void ResponseHandler::__messageReceipted(const std::string &message)
{
    std::string msg, from;

    if (MessageParser::parseMessageReceipt(message, msg, from)) {
        if (std::getenv("ENV") && std::string(std::getenv("ENV")) == "dev") std::cout << "\n\n╭─ \033[36mmyteams\033[0m from \033[36m" << from << "\033[0m\n╰─$ " << msg.substr(1, msg.size() - 2) << std::endl;
        else client_event_private_message_received(from.c_str(), msg.c_str());
    } else MessageParser::printResponseFailure(message);
}

void ResponseHandler::__threadCreationReceipted(const std::string &message)
{
    std::string thread_uuid, user_uuid, thread_title, thread_body;
    time_t thread_timestamp;

    if (MessageParser::parseThreadCreationReceipt(message, thread_uuid, user_uuid, thread_timestamp, thread_title, thread_body)) {
        if (std::getenv("ENV") && std::string(std::getenv("ENV")) == "dev") std::cout << "\n\n╭─ \033[35mmyteams\033[0m new Thread: \033[35m" << thread_uuid << "\033[0m\n╰─$ " << thread_title << ": " << thread_body << std::endl;
        else client_event_thread_created(thread_uuid.c_str(), user_uuid.c_str(), thread_timestamp, thread_title.c_str(), thread_body.c_str());
    } else MessageParser::printResponseFailure(message);
}

void ResponseHandler::__threadReplyCreationReceipted(const std::string &message)
{
    std::string team_uuid, thread_uuid, user_uuid, reply_body;

    if (MessageParser::parseThreadReplyCreationReceipt(message, team_uuid, thread_uuid, user_uuid, reply_body)) {
        if (std::getenv("ENV") && std::string(std::getenv("ENV")) == "dev") std::cout << "\n\n╭─ \033[35mmyteams\033[0m new Reply in thread: \033[35m" << thread_uuid << "\033[0m\n╰─$ " << reply_body << std::endl;
        else client_event_thread_reply_received(team_uuid.c_str(), thread_uuid.c_str(), user_uuid.c_str(), reply_body.c_str());
    } else MessageParser::printResponseFailure(message);
}

void ResponseHandler::__teamCreationReceipted(const std::string &message)
{
    std::string team_uuid, team_name, team_description;

    if (MessageParser::parseTeamCreationReceipt(message, team_uuid, team_name, team_description)) {
        if (std::getenv("ENV") && std::string(std::getenv("ENV")) == "dev") std::cout << "\n\n╭─ \033[35mmyteams\033[0m new Team: \033[35m" << team_uuid << "\033[0m\n╰─$ " << team_name << " - " << team_description << std::endl;
        else client_event_team_created(team_uuid.c_str(), team_name.c_str(), team_description.c_str());
    } else MessageParser::printResponseFailure(message);
}

void ResponseHandler::__channelCreationReceipted(const std::string &message)
{
    std::string channel_uuid, channel_name, channel_description;

    if (MessageParser::parseChannelCreationReceipt(message, channel_uuid, channel_name, channel_description)) {
        if (std::getenv("ENV") && std::string(std::getenv("ENV")) == "dev") std::cout << "\n\n╭─ \033[35mmyteams\033[0m new Channel: \033[35m" << channel_uuid << "\033[0m\n╰─$ " << channel_name << " - " << channel_description << std::endl;
        else client_event_channel_created(channel_uuid.c_str(), channel_name.c_str(), channel_description.c_str());
    } else MessageParser::printResponseFailure(message);
}

bool ResponseHandler::__async_gestion(const std::string &code, const std::string &message)
{
    if (code == "225") {
        __messageReceipted(message);
        return true;
    }
    if (code == "232") {
        __threadCreationReceipted(message);
        return true;
    }
    if (code == "233") {
        __threadReplyCreationReceipted(message);
        return true;
    }
    if (code == "234") {
        __teamCreationReceipted(message);
        return true;
    }
    if (code == "235") {
        __channelCreationReceipted(message);
        return true;
    }
    return false;
}

void ResponseHandler::__processResponseCode(const std::string &code, const std::string &message)
{
    if (__async_gestion(code, message)) return;
    if (code == "201" || code == "203") {
        _waiting_for_202 = true;
    } else if (_waiting_for_202 && code == "202") {
        _waiting_for_202 = false;
        return;
    }
    auto handler = LoggingHandlerFactory::createHandler(_last_command, _last_args, _context);
    if (handler) {
        handler->handleResponse(code, message);
    } else {
        MessageParser::printResponsePrompt(code);
        std::cout << message << std::endl;
    }
}

void ResponseHandler::__splitAndProcessLine(const std::string &line)
{
    std::string code, message;

    __extractCodeAndMessage(line, code, message);
    __processResponseCode(code, message);
}

void ResponseHandler::handleServerResponse(const std::string &raw_response)
{
    std::string line;

    for (size_t i = 0; i <= raw_response.size(); i++) {
        if (i == raw_response.size() || raw_response[i] == '\n' || raw_response[i] == '\r') {
            if (!line.empty()) __splitAndProcessLine(line);
            line.clear();
            continue;
        }
        line += raw_response[i];
    }
}
