#include "class/CommandLoggingHandlers.hpp"
#include "class/MessageParser.hpp"
#include "logging_client.h"
#include <iostream>

// ----- TEAM ----- //

void ListTeamsHandler::handleResponse(const std::string &code, const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "201") return __handle201(message);
	if (code == "530") return __handle530();
	std::cout << message << std::endl;
}

void ListTeamsHandler::__handle201(const std::string &message) const
{
	std::string team_name, team_uuid, team_description;

	if (MessageParser::parseTeamInfo(message, team_name, team_uuid, team_description)) client_print_teams(team_uuid.c_str(), team_name.c_str(), team_description.c_str());
	else MessageParser::printResponseFailure(message);
}

void ListTeamsHandler::__handle530() const { client_error_unauthorized(); }

// ----- CHANNELS ----- //

void ListChannelsHandler::handleResponse(const std::string &code, const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "201") return __handle201(message);
	if (code == "404") return __handle404();
	if (code == "530") return __handle530();
	std::cout << message << std::endl;
}

void ListChannelsHandler::__handle201(const std::string &message) const
{
	std::string channel_name, channel_uuid, channel_description;

	if (MessageParser::parseChannelInfo(message, channel_name, channel_uuid, channel_description)) client_team_print_channels(channel_uuid.c_str(), channel_name.c_str(), channel_description.c_str());
	else MessageParser::printResponseFailure(message);
}

void ListChannelsHandler::__handle404() const
{
	std::string target_uuid = _context.getTeamUuid();

	client_error_unknown_team(target_uuid.c_str());
}

void ListChannelsHandler::__handle530() const { client_error_unauthorized(); }

// ----- THREADS ----- //

void ListThreadsHandler::handleResponse(const std::string &code, const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "201") return __handle201(message);
	if (code == "404") return __handle404();
	if (code == "405") return __handle405();
	if (code == "530") return __handle530();
	std::cout << message << std::endl;
}

void ListThreadsHandler::__handle201(const std::string &message) const
{
	std::string thread_uuid, user_uuid, thread_title, thread_body;
	long timestamp = 0;

	if (MessageParser::parseThreadInfo(message, thread_uuid, user_uuid, timestamp, thread_title, thread_body)) client_channel_print_threads(thread_uuid.c_str(), user_uuid.c_str(), static_cast<time_t>(timestamp), thread_title.c_str(), thread_body.c_str());
	else MessageParser::printResponseFailure(message);
}

void ListThreadsHandler::__handle404() const
{
	std::string target_uuid = _context.getTeamUuid();

	client_error_unknown_team(target_uuid.c_str());
}

void ListThreadsHandler::__handle405() const
{
	std::string target_uuid = _context.getChannelUuid();

	client_error_unknown_channel(target_uuid.c_str());
}

void ListThreadsHandler::__handle530() const { client_error_unauthorized(); }

// ----- COMMENTS ----- //

void ListCommentsHandler::handleResponse(const std::string &code, const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "201") return __handle201(message);
	if (code == "404") return __handle404();
	if (code == "405") return __handle405();
	if (code == "406") return __handle406();
	if (code == "530") return __handle530();
	std::cout << message << std::endl;
}

void ListCommentsHandler::__handle201(const std::string &message) const
{
	std::string thread_uuid, user_uuid, reply_body;
	long timestamp = 0;

	if (MessageParser::parseThreadReplyInfo(message, thread_uuid, user_uuid, timestamp, reply_body)) client_thread_print_replies(thread_uuid.c_str(), user_uuid.c_str(), static_cast<time_t>(timestamp), reply_body.c_str());
	else MessageParser::printResponseFailure(message);
}

void ListCommentsHandler::__handle404() const
{
	std::string target_uuid = _context.getTeamUuid();

	client_error_unknown_team(target_uuid.c_str());
}

void ListCommentsHandler::__handle405() const
{
	std::string target_uuid = _context.getChannelUuid();

	client_error_unknown_channel(target_uuid.c_str());
}

void ListCommentsHandler::__handle406() const
{
	std::string target_uuid = _context.getThreadUuid();

	client_error_unknown_thread(target_uuid.c_str());
}

void ListCommentsHandler::__handle530() const { client_error_unauthorized(); }
