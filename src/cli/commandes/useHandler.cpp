/*
** EPITECH PROJECT, 2026
** MyTeams
** File description:
** Use (context) logging handler implementation
*/

#include "class/CommandLoggingHandlers.hpp"
#include "class/MessageParser.hpp"
#include "logging_client.h"
#include <iostream>

// ==================== USE HANDLER ====================

void UseHandler::handleResponse(const std::string &code,
	const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "200") __handle200();
	else if (code == "404") __handle404();
	else if (code == "405") __handle405();
	else if (code == "406") __handle406();
	else if (code == "530") __handle530();
	else std::cout << message << std::endl;
}

void UseHandler::__handle200() const
{
	// Context successfully updated - no specific logging function
}

void UseHandler::__handle404() const
{
	// Team not found
	client_error_unknown_team("");
}

void UseHandler::__handle405() const
{
	// Channel not found
	client_error_unknown_channel("");
}

void UseHandler::__handle406() const
{
	// Thread not found
	client_error_unknown_thread("");
}

void UseHandler::__handle530() const
{
	client_error_unauthorized();
}
