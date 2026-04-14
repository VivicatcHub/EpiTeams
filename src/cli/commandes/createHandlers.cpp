#include "class/CommandLoggingHandlers.hpp"
#include "class/MessageParser.hpp"
#include "logging_client.h"
#include <iostream>

void CreateTeamHandler::handleResponse(const std::string &code, const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "200") __handle200(message);
	else if (code == "530") __handle530();
	else std::cout << message << std::endl;
}

void CreateTeamHandler::__handle200(const std::string &message) const
{
	std::string team_name, team_uuid, team_description;

	if (MessageParser::parseTeamInfo(message, team_name, team_uuid, team_description)) client_print_team_created(team_uuid.c_str(), team_name.c_str(), team_description.c_str());
}

void CreateTeamHandler::__handle530() const
{
	client_error_unauthorized();
}

void CreateChannelHandler::handleResponse(const std::string &code, const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "200") __handle200(message);
	else if (code == "404") __handle404();
	else if (code == "530") __handle530();
	else std::cout << message << std::endl;
}

void CreateChannelHandler::__handle200(const std::string &message) const
{
	std::string channel_name, channel_uuid, channel_description;

	if (MessageParser::parseChannelInfo(message, channel_name, channel_uuid, channel_description)) client_print_channel_created(channel_uuid.c_str(), channel_name.c_str(), channel_description.c_str());
}

void CreateChannelHandler::__handle404() const { client_error_unknown_team(""); }

void CreateChannelHandler::__handle530() const { client_error_unauthorized(); }

void CreateThreadHandler::handleResponse(const std::string &code, const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "200") __handle200(message);
	else if (code == "404") __handle404();
	else if (code == "405") __handle405();
	else if (code == "530") __handle530();
	else std::cout << message << std::endl;
}

void CreateThreadHandler::__handle200(const std::string &message) const
{
	std::string thread_uuid, user_uuid;
	long timestamp = 0;
	std::string thread_title, thread_body;

	if (MessageParser::parseThreadInfo(message, thread_uuid, user_uuid, timestamp, thread_title, thread_body)) client_print_thread_created(thread_uuid.c_str(), user_uuid.c_str(), static_cast<time_t>(timestamp), thread_title.c_str(), thread_body.c_str());
}

void CreateThreadHandler::__handle404() const { client_error_unknown_team(""); }

void CreateThreadHandler::__handle405() const { client_error_unknown_channel(""); }

void CreateThreadHandler::__handle530() const { client_error_unauthorized(); }

void CreateCommentHandler::handleResponse(const std::string &code, const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "200") __handle200();
	else if (code == "404") __handle404();
	else if (code == "405") __handle405();
	else if (code == "406") __handle406();
	else if (code == "530") __handle530();
	else std::cout << message << std::endl;
}

void CreateCommentHandler::__handle200() const
{
}

void CreateCommentHandler::__handle404() const { client_error_unknown_team(""); }

void CreateCommentHandler::__handle405() const { client_error_unknown_channel(""); }

void CreateCommentHandler::__handle406() const { client_error_unknown_thread(""); }

void CreateCommentHandler::__handle530() const { client_error_unauthorized(); }
