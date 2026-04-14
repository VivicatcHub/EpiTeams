#include "class/CommandLoggingHandlers.hpp"
#include "class/MessageParser.hpp"
#include "logging_client.h"
#include <iostream>

// ==================== LIST_TEAMS HANDLER ====================

void ListTeamsHandler::handleResponse(const std::string &code,
	const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "200") __handle200(message);
	else if (code == "530") __handle530();
	else std::cout << message << std::endl;
}

void ListTeamsHandler::__handle200(const std::string &message) const
{
	std::string team_name, team_uuid, team_description;

	if (MessageParser::parseTeamInfo(message, team_name, team_uuid, team_description)) {
		client_print_teams(team_uuid.c_str(), team_name.c_str(), team_description.c_str());
	}
}

void ListTeamsHandler::__handle530() const
{
	client_error_unauthorized();
}

// ==================== LIST_CHANNELS HANDLER ====================

void ListChannelsHandler::handleResponse(const std::string &code,
	const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "200") __handle200(message);
	else if (code == "404") __handle404();
	else if (code == "530") __handle530();
	else std::cout << message << std::endl;
}

void ListChannelsHandler::__handle200(const std::string &message) const
{
	std::string channel_name, channel_uuid, channel_description;

	if (MessageParser::parseChannelInfo(message, channel_name, channel_uuid, channel_description)) {
		client_team_print_channels(channel_uuid.c_str(), channel_name.c_str(), channel_description.c_str());
	}
}

void ListChannelsHandler::__handle404() const
{
	// Team not found
	client_error_unknown_team("");
}

void ListChannelsHandler::__handle530() const
{
	client_error_unauthorized();
}

// ==================== LIST_THREADS HANDLER ====================

void ListThreadsHandler::handleResponse(const std::string &code,
	const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "200") __handle200(message);
	else if (code == "404") __handle404();
	else if (code == "405") __handle405();
	else if (code == "530") __handle530();
	else std::cout << message << std::endl;
}

void ListThreadsHandler::__handle200(const std::string &message) const
{
	std::string thread_uuid, user_uuid;
	long timestamp = 0;
	std::string thread_title, thread_body;

	if (MessageParser::parseThreadInfo(message, thread_uuid, user_uuid, timestamp, thread_title, thread_body)) {
		client_channel_print_threads(thread_uuid.c_str(), user_uuid.c_str(),
			static_cast<time_t>(timestamp), thread_title.c_str(), thread_body.c_str());
	}
}

void ListThreadsHandler::__handle404() const
{
	// Team or channel not found
	client_error_unknown_team("");
}

void ListThreadsHandler::__handle405() const
{
	// Channel not found
	client_error_unknown_channel("");
}

void ListThreadsHandler::__handle530() const
{
	client_error_unauthorized();
}

// ==================== LIST_COMMENTS HANDLER ====================

void ListCommentsHandler::handleResponse(const std::string &code,
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

void ListCommentsHandler::__handle200(const std::string &message) const
{
	std::string thread_uuid, user_uuid;
	long timestamp = 0;
	std::string reply_body;

	// Format: thread_uuid user_uuid timestamp "reply_body"
	// For now, we parse like a private message format
	if (MessageParser::parsePrivateMessage(message, user_uuid, timestamp, reply_body)) {
		client_thread_print_replies(thread_uuid.c_str(), user_uuid.c_str(),
			static_cast<time_t>(timestamp), reply_body.c_str());
	}
}

void ListCommentsHandler::__handle404() const
{
	// Team, channel, or thread not found
	client_error_unknown_team("");
}

void ListCommentsHandler::__handle405() const
{
	// Channel not found
	client_error_unknown_channel("");
}

void ListCommentsHandler::__handle406() const
{
	// Thread not found
	client_error_unknown_thread("");
}

void ListCommentsHandler::__handle530() const
{
	client_error_unauthorized();
}
