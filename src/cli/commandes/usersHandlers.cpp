#include "class/CommandLoggingHandlers.hpp"
#include "class/MessageParser.hpp"
#include "logging_client.h"
#include <iostream>

void UsersHandler::handleResponse(const std::string &code, const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "200") return __handle200(message);
	if (code == "530") return __handle530();
	std::cout << message << std::endl;
}

void UsersHandler::__handle200(const std::string &message) const
{
	std::string user_name, user_uuid;
	int user_status = 0;

	if (MessageParser::parseUserStatus(message, user_name, user_uuid, user_status)) client_print_users(user_uuid.c_str(), user_name.c_str(), user_status);
	else MessageParser::printResponseFailure(message);
}

void UsersHandler::__handle530() const { client_error_unauthorized(); }
