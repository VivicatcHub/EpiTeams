#include "class/CommandLoggingHandlers.hpp"
#include "class/MessageParser.hpp"
#include "logging_client.h"
#include <iostream>

// ==================== INFO_USER HANDLER ====================

void InfoUserHandler::handleResponse(const std::string &code,
	const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "200") __handle200(message);
	else if (code == "530") __handle530();
	else std::cout << message << std::endl;
}

void InfoUserHandler::__handle200(const std::string &message) const
{
	std::string user_name, user_uuid;
	int user_status = 0;

	if (MessageParser::parseUserStatus(message, user_name, user_uuid, user_status)) {
		client_print_user(user_uuid.c_str(), user_name.c_str(), user_status);
	}
}

void InfoUserHandler::__handle530() const
{
	client_error_unauthorized();
}

// ==================== INFO_TEAM HANDLER ====================

void InfoTeamHandler::handleResponse(const std::string &code,
	const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "200") __handle200(message);
	else if (code == "404") __handle404();
	else if (code == "530") __handle530();
	else std::cout << message << std::endl;
}

void InfoTeamHandler::__handle200(const std::string &message) const
{
	std::string team_name, team_uuid, team_description;

	if (MessageParser::parseTeamInfo(message, team_name, team_uuid, team_description)) {
		client_print_team(team_uuid.c_str(), team_name.c_str(), team_description.c_str());
	}
}

void InfoTeamHandler::__handle404() const
{
	// Team not found
	client_error_unknown_team("");
}

void InfoTeamHandler::__handle530() const
{
	client_error_unauthorized();
}

// ==================== INFO_CHANNEL HANDLER ====================

void InfoChannelHandler::handleResponse(const std::string &code,
	const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "200") __handle200(message);
	else if (code == "404") __handle404();
	else if (code == "405") __handle405();
	else if (code == "530") __handle530();
	else std::cout << message << std::endl;
}

void InfoChannelHandler::__handle200(const std::string &message) const
{
	std::string channel_name, channel_uuid, channel_description;

	if (MessageParser::parseChannelInfo(message, channel_name, channel_uuid, channel_description)) {
		client_print_channel(channel_uuid.c_str(), channel_name.c_str(), channel_description.c_str());
	}
}

void InfoChannelHandler::__handle404() const
{
	// Team not found
	client_error_unknown_team("");
}

void InfoChannelHandler::__handle405() const
{
	// Channel not found
	client_error_unknown_channel("");
}

void InfoChannelHandler::__handle530() const
{
	client_error_unauthorized();
}

// ==================== INFO_THREAD HANDLER ====================

void InfoThreadHandler::handleResponse(const std::string &code,
	const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "200") __handle200(message);
	else if (code == "404") __handle404();
	else if (code == "405") __handle405();
	else if (code == "406") __handle406();
	else if (code == "530") __handle530();
	else std::cout << message << std::endl;
}

void InfoThreadHandler::__handle200(const std::string &message) const
{
	std::string thread_uuid, user_uuid;
	long timestamp = 0;
	std::string thread_title, thread_body;

	if (MessageParser::parseThreadInfo(message, thread_uuid, user_uuid, timestamp, thread_title, thread_body)) {
		client_print_thread(thread_uuid.c_str(), user_uuid.c_str(),
			static_cast<time_t>(timestamp), thread_title.c_str(), thread_body.c_str());
	}
}

void InfoThreadHandler::__handle404() const
{
	// Team, channel, or thread not found
	client_error_unknown_team("");
}

void InfoThreadHandler::__handle405() const
{
	// Channel not found
	client_error_unknown_channel("");
}

void InfoThreadHandler::__handle406() const
{
	// Thread not found
	client_error_unknown_thread("");
}

void InfoThreadHandler::__handle530() const
{
	client_error_unauthorized();
}
