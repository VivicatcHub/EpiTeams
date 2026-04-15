#include "class/CommandLoggingHandlers.hpp"
#include "class/MessageParser.hpp"
#include "logging_client.h"
#include <iostream>

void LoginHandler::handleResponse(const std::string &code, const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "230") return __handle230(message);
	if (code == "501") __handle501();
	if (code == "503") __handle503();
	if (code == "540") __handle540();
	std::cout << message << std::endl;
}

void LoginHandler::__handle230(const std::string &message) const
{
	std::string user_name, user_uuid;

	if (MessageParser::parseLoginSuccess(message, user_name, user_uuid)) client_event_logged_in(user_uuid.c_str(), user_name.c_str());
	else MessageParser::printResponseFailure(message);
}

void LoginHandler::__handle501() const {}

void LoginHandler::__handle503() const {}

void LoginHandler::__handle540() const {}
