#include "class/CommandLoggingHandlers.hpp"
#include "class/MessageParser.hpp"
#include "logging_client.h"
#include <iostream>


void SubscribeHandler::handleResponse(const std::string &code, const std::string &message) const
{
    MessageParser::printResponsePrompt(code);

    if (code == "200") __handle200();
    if (code == "404") return __handle404();
    if (code == "530") return __handle530();
    std::cout << message << std::endl;
}

void SubscribeHandler::__handle200() const {}

void SubscribeHandler::__handle404() const
{
    std::string target_uuid = _args.empty() ? "" : _args[0];
    client_error_unknown_team(target_uuid.c_str());
}

void SubscribeHandler::__handle530() const { client_error_unauthorized(); }
