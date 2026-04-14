#include "class/CommandLoggingHandlers.hpp"
#include "class/MessageParser.hpp"
#include "logging_client.h"
#include <iostream>

void UserHandler::handleResponse(const std::string &code, const std::string &message) const
{
    MessageParser::printResponsePrompt(code);

    if (code == "200") return __handle200(message);
    if (code == "404") return __handle404(message);
    if (code == "530") return __handle530();
    std::cout << message << std::endl;
}

void UserHandler::__handle200(const std::string &message) const
{
    std::string user_name, user_uuid;
    int user_status = 0;

    if (MessageParser::parseUserStatus(message, user_name, user_uuid, user_status)) client_print_user(user_uuid.c_str(), user_name.c_str(), user_status);
    else MessageParser::printResponseFailure(message);
}

void UserHandler::__handle404(const std::string &message) const {
    std::string user_uuid;

    if (!MessageParser::extractUuid(message).empty()) client_error_unknown_user(user_uuid.c_str());
    else MessageParser::printResponseFailure(message);
}

void UserHandler::__handle530() const { client_error_unauthorized(); }
