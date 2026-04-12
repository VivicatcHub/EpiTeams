#include "class/ResponseHandler.hpp"
#include "logging_client.h"
#include <cctype>
#include <iostream>

ResponseHandler::ResponseHandler() {}

std::string ResponseHandler::getLastCommand() const { return _last_command; }

std::vector<std::string> ResponseHandler::getLastArgs() const { return _last_args; }

void ResponseHandler::setCommandContext(const std::string &command,
    const std::vector<std::string> &args)
{
    _last_command = command;
    _last_args = args;
}

std::string ResponseHandler::__cleanString(const std::string &value)
{
    size_t start = 0;
    size_t end = value.size();

    while (start < value.size() && std::isspace(static_cast<unsigned char>(value[start]))) start++;
    while (end > start && std::isspace(static_cast<unsigned char>(value[end - 1]))) end--;
    return value.substr(start, end - start);
}

bool ResponseHandler::__extractCodeAndMessage(const std::string &line,
    std::string &code, std::string &message)
{
    std::string cleaned = __cleanString(line);

    if (cleaned.empty()) return false;
    if (cleaned.size() < 3
        || !std::isdigit(static_cast<unsigned char>(cleaned[0]))
        || !std::isdigit(static_cast<unsigned char>(cleaned[1]))
        || !std::isdigit(static_cast<unsigned char>(cleaned[2]))) {
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

bool ResponseHandler::__parseUserStatusMessage(const std::string &message,
    std::string &user_name, std::string &user_uuid, int &user_status)
{
    const std::string open = " (";
    const std::string middle = ") -> ";
    size_t open_pos = message.find(open);
    size_t middle_pos = message.find(middle);
    std::string status;

    if (open_pos == std::string::npos || middle_pos == std::string::npos) return false;
    if (open_pos == 0 || middle_pos <= open_pos + open.size()) return false;

    user_name = message.substr(0, open_pos);
    user_uuid = message.substr(open_pos + open.size(),
        middle_pos - (open_pos + open.size()));
    status = message.substr(middle_pos + middle.size());

    if (status == "ONLINE") user_status = 1;
    else if (status == "OFFLINE") user_status = 0;
    else return false;

    return !user_name.empty() && !user_uuid.empty();
}

void ResponseHandler::__printStyled(const std::string &code,
    const std::string &message) const
{
    if (const char *env = std::getenv("ENV")) {
        if (std::string(env) == "dev") {
            std::cout << "  ╰─> [" << code << "]: " << message << std::endl;
            return;
        }
    }
    std::cout << "  ╰─> " << message << std::endl;
}

void ResponseHandler::__dispatchLogging(const std::string &code,
    const std::string &message) const
{
    std::string user_name;
    std::string user_uuid;
    int user_status = 0;
    const char *context_uuid = _last_args.empty() ? "" : _last_args[0].c_str();

    if (code == "530") {
        client_error_unauthorized();
        return;
    }
    if (code == "540") {
        client_error_already_exist();
        return;
    }
    if (code == "405" || code == "406") {
        client_error_unknown_channel(context_uuid);
        return;
    }
    if (code == "404") {
        if (message.find("User not found") != std::string::npos)
            client_error_unknown_user(context_uuid);
        else if (message.find("Team not found") != std::string::npos)
            client_error_unknown_team(context_uuid);
        else if (message.find("Channel not found") != std::string::npos)
            client_error_unknown_channel(context_uuid);
        else if (message.find("Thread not found") != std::string::npos)
            client_error_unknown_thread(context_uuid);
        return;
    }
    if (code == "200" && __parseUserStatusMessage(message,
        user_name, user_uuid, user_status)) {
        if (_last_command == "users")
            client_print_users(user_uuid.c_str(), user_name.c_str(), user_status);
        else if (_last_command == "user")
            client_print_user(user_uuid.c_str(), user_name.c_str(), user_status);
    }
}

void ResponseHandler::handleServerResponse(const std::string &raw_response) const
{
    std::string line;

    for (size_t i = 0; i <= raw_response.size(); i++) {
        if (i == raw_response.size() || raw_response[i] == '\n' || raw_response[i] == '\r') {
            std::string code;
            std::string message;

            if (__extractCodeAndMessage(line, code, message)) {
                __printStyled(code, message);
                __dispatchLogging(code, message);
            }
            line.clear();
            continue;
        }
        line += raw_response[i];
    }
}
