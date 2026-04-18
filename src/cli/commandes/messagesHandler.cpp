#include "class/CommandLoggingHandlers.hpp"
#include "class/MessageParser.hpp"
#include "logging_client.h"
#include <iostream>

void MessagesHandler::handleResponse(const std::string &code, const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "201") return __handle201(message);
	if (code == "404") return __handle404();
	if (code == "501") return __handle501();
	if (code == "530") return __handle530();
	std::cout << message << std::endl;
}

void MessagesHandler::__handle201(const std::string &message) const
{
	std::string sender_uuid, message_body;
	long timestamp = 0;

	if (MessageParser::parsePrivateMessage(message, sender_uuid, timestamp, message_body)) client_private_message_print_messages(sender_uuid.c_str(), static_cast<time_t>(timestamp), message_body.c_str());
	else MessageParser::printResponseFailure(message);
}

void MessagesHandler::__handle404() const
{
	std::string target_uuid = _args.empty() ? "" : _args[0];

	client_error_unknown_user(target_uuid.c_str());
}

void MessagesHandler::__handle501() const
{
	std::string target_uuid = _args.empty() ? "" : _args[0];

	client_error_unknown_user(target_uuid.c_str());
}

void MessagesHandler::__handle530() const { client_error_unauthorized(); }
