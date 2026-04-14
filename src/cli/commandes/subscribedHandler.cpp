#include "class/CommandLoggingHandlers.hpp"
#include "class/MessageParser.hpp"
#include "logging_client.h"
#include <iostream>


void SubscribedHandler::handleResponse(const std::string &code, const std::string &message) const
{
    MessageParser::printResponsePrompt(code);

    if (code == "201") return __handle201(message);
    if (code == "203") return __handle203(message);
    if (code == "404") return __handle404();
    if (code == "530") return __handle530();
    std::cout << message << std::endl;
}

void SubscribedHandler::__handle201(const std::string &message) const
{
    std::string team_name, team_uuid, team_description;

    if (MessageParser::parseTeamInfo(message, team_name, team_uuid, team_description)) client_print_teams(team_uuid.c_str(), team_name.c_str(), team_description.c_str());
    else MessageParser::printResponseFailure(message);
}

void SubscribedHandler::__handle203(const std::string &message) const
{
    std::string user_name, user_uuid;
    int user_status = 0;

    if (MessageParser::parseUserStatus(message, user_name, user_uuid, user_status)) client_print_users(user_uuid.c_str(), user_name.c_str(), user_status);
    else MessageParser::printResponseFailure(message);
}

void SubscribedHandler::__handle404() const
{
    std::string target_uuid = _args.empty() ? "" : _args[0];

    client_error_unknown_team(target_uuid.c_str());
}

void SubscribedHandler::__handle530() const { client_error_unauthorized(); }
