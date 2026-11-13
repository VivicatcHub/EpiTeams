#include "class/CommandLoggingHandlers.hpp"
#include "class/MessageParser.hpp"
#include "logging_client.h"
#include <iostream>

void LogoutHandler::handleResponse(const std::string &code, const std::string &message) const
{
    MessageParser::printResponsePrompt(code);

    if (code == "231") return __handle231(message);
    if (code == "530") return __handle530();
    std::cout << message << std::endl;
}

void LogoutHandler::__handle231(const std::string &message) const
{
    std::string user_name, user_uuid;

    if (MessageParser::parseLogoutSuccess(message, user_name, user_uuid)) client_event_logged_out(user_uuid.c_str(), user_name.c_str());
    else MessageParser::printResponseFailure(message);
}

void LogoutHandler::__handle530() const { client_error_unauthorized(); }
