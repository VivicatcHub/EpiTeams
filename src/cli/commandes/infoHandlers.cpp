#include "class/CommandLoggingHandlers.hpp"
#include "class/MessageParser.hpp"
#include "logging_client.h"
#include <iostream>

// ----- USER ----- //

void InfoUserHandler::handleResponse(const std::string &code, const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "201") return __handle201(message);
	if (code == "530") return __handle530();
	std::cout << message << std::endl;
}

void InfoUserHandler::__handle201(const std::string &message) const
{
	std::string user_name, user_uuid;
	int user_status = 0;

	if (MessageParser::parseUserStatus(message, user_name, user_uuid, user_status)) client_print_user(user_uuid.c_str(), user_name.c_str(), user_status);
	else MessageParser::printResponseFailure(message);
}

void InfoUserHandler::__handle530() const { client_error_unauthorized(); }

// ----- TEAM ----- //

void InfoTeamHandler::handleResponse(const std::string &code, const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "201") return __handle201(message);
	if (code == "404") return __handle404();
	if (code == "530") return __handle530();
	std::cout << message << std::endl;
}

void InfoTeamHandler::__handle201(const std::string &message) const
{
	std::string team_name, team_uuid, team_description;

	if (MessageParser::parseTeamInfo(message, team_name, team_uuid, team_description)) client_print_team(team_uuid.c_str(), team_name.c_str(), team_description.c_str());
	else MessageParser::printResponseFailure(message);
}

void InfoTeamHandler::__handle404() const
{
	std::string target_uuid = _context.getTeamUuid();

	client_error_unknown_team(target_uuid.c_str());
}

void InfoTeamHandler::__handle530() const { client_error_unauthorized(); }

// ----- CHANNEL ----- //

void InfoChannelHandler::handleResponse(const std::string &code, const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "201") return __handle201(message);
	if (code == "404") return __handle404();
	if (code == "405") return __handle405();
	if (code == "530") return __handle530();
	std::cout << message << std::endl;
}

void InfoChannelHandler::__handle201(const std::string &message) const
{
	std::string channel_name, channel_uuid, channel_description;

	if (MessageParser::parseChannelInfo(message, channel_name, channel_uuid, channel_description)) client_print_channel(channel_uuid.c_str(), channel_name.c_str(), channel_description.c_str());
	else MessageParser::printResponseFailure(message);
}

void InfoChannelHandler::__handle404() const
{
	std::string target_uuid = _context.getTeamUuid();

	client_error_unknown_team(target_uuid.c_str());
}

void InfoChannelHandler::__handle405() const
{
	std::string target_uuid = _context.getChannelUuid();

	client_error_unknown_channel(target_uuid.c_str());
}

void InfoChannelHandler::__handle530() const { client_error_unauthorized(); }

// ----- THREAD ----- //

void InfoThreadHandler::handleResponse(const std::string &code, const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "201") return __handle201(message);
	if (code == "404") return __handle404();
	if (code == "405") return __handle405();
	if (code == "406") return __handle406();
	if (code == "530") return __handle530();
	std::cout << message << std::endl;
}

void InfoThreadHandler::__handle201(const std::string &message) const
{
	std::string thread_uuid, user_uuid, thread_title, thread_body;
	long timestamp = 0;

	if (MessageParser::parseThreadInfo(message, thread_uuid, user_uuid, timestamp, thread_title, thread_body)) client_print_thread(thread_uuid.c_str(), user_uuid.c_str(), static_cast<time_t>(timestamp), thread_title.c_str(), thread_body.c_str());
	else MessageParser::printResponseFailure(message);
}

void InfoThreadHandler::__handle404() const
{
	std::string target_uuid = _context.getTeamUuid();

	client_error_unknown_team(target_uuid.c_str());
}

void InfoThreadHandler::__handle405() const
{
	std::string target_uuid = _context.getChannelUuid();

	client_error_unknown_channel(target_uuid.c_str());
}

void InfoThreadHandler::__handle406() const
{
	std::string target_uuid = _context.getThreadUuid();

	client_error_unknown_thread(target_uuid.c_str());
}

void InfoThreadHandler::__handle530() const { client_error_unauthorized(); }
