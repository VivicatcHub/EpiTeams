#include "class/CommandLoggingHandlers.hpp"
#include "class/MessageParser.hpp"
#include "logging_client.h"
#include <iostream>

void UnsubscribeHandler::handleResponse(const std::string &code, const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "200") return __handle200(message);
	if (code == "404") return __handle404();
	if (code == "530") return __handle530();
	std::cout << message << std::endl;
}

void UnsubscribeHandler::__handle200(const std::string &message) const
{
	std::string team_uuid, user_uuid;

	if (MessageParser::parseUnsubscribeResponse(message, team_uuid, user_uuid)) client_print_unsubscribed(user_uuid.c_str(), team_uuid.c_str());
	else MessageParser::printResponseFailure(message);
}

void UnsubscribeHandler::__handle404() const
{
	std::string target_uuid = _args.empty() ? "" : _args[0];

	client_error_unknown_team(target_uuid.c_str());
}

void UnsubscribeHandler::__handle530() const { client_error_unauthorized(); }
