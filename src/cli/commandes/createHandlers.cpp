#include "class/CommandLoggingHandlers.hpp"
#include "class/MessageParser.hpp"
#include "logging_client.h"
#include <iostream>

// ----- TEAM ----- //

void CreateTeamHandler::handleResponse(const std::string &code, const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "200") return __handle200(message);
	if (code == "530") return __handle530();
	std::cout << message << std::endl;
}

void CreateTeamHandler::__handle200(const std::string &message) const
{
	std::string team_name, team_uuid, team_description;

	if (MessageParser::parseTeamInfo(message, team_name, team_uuid, team_description)) client_print_team_created(team_uuid.c_str(), team_name.c_str(), team_description.c_str());
	else MessageParser::printResponseFailure(message);
}

void CreateTeamHandler::__handle530() const { client_error_unauthorized(); }

// ----- CHANNEL ----- //

void CreateChannelHandler::handleResponse(const std::string &code, const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "200") return __handle200(message);
	if (code == "404") return __handle404();
	if (code == "530") return __handle530();
	std::cout << message << std::endl;
}

void CreateChannelHandler::__handle200(const std::string &message) const
{
	std::string channel_name, channel_uuid, channel_description;

	if (MessageParser::parseChannelInfo(message, channel_name, channel_uuid, channel_description)) client_print_channel_created(channel_uuid.c_str(), channel_name.c_str(), channel_description.c_str());
	else MessageParser::printResponseFailure(message);
}

void CreateChannelHandler::__handle404() const
{
	std::string target_uuid = _args.empty() ? "" : _args[0];
	client_error_unknown_team(target_uuid.c_str());
}

void CreateChannelHandler::__handle530() const { client_error_unauthorized(); }

// ----- THREAD ----- //

void CreateThreadHandler::handleResponse(const std::string &code, const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "200") return __handle200(message);
	if (code == "404") return __handle404();
	if (code == "405") return __handle405();
	if (code == "530") return __handle530();
	std::cout << message << std::endl;
}

void CreateThreadHandler::__handle200(const std::string &message) const
{
	std::string thread_uuid, user_uuid;
	long timestamp = 0;
	std::string thread_title, thread_body;

	if (MessageParser::parseThreadInfo(message, thread_uuid, user_uuid, timestamp, thread_title, thread_body)) client_print_thread_created(thread_uuid.c_str(), user_uuid.c_str(), static_cast<time_t>(timestamp), thread_title.c_str(), thread_body.c_str());
	else MessageParser::printResponseFailure(message);
}

void CreateThreadHandler::__handle404() const
{
	std::string target_uuid = _args.empty() ? "" : _args[0];
	client_error_unknown_team(target_uuid.c_str());
}

void CreateThreadHandler::__handle405() const
{
	std::string target_uuid = _args.size() > 1 ? _args[1] : "";
	client_error_unknown_channel(target_uuid.c_str());
}

void CreateThreadHandler::__handle530() const { client_error_unauthorized(); }

// ----- COMMENT ----- //

void CreateCommentHandler::handleResponse(const std::string &code, const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "200") __handle200();
	if (code == "404") return __handle404();
	if (code == "405") return __handle405();
	if (code == "406") return __handle406();
	if (code == "530") return __handle530();
	std::cout << message << std::endl;
}

void CreateCommentHandler::__handle200() const {}

void CreateCommentHandler::__handle404() const
{
	std::string target_uuid = _args.empty() ? "" : _args[0];
	client_error_unknown_team(target_uuid.c_str());
}

void CreateCommentHandler::__handle405() const
{
	std::string target_uuid = _args.size() > 1 ? _args[1] : "";
	client_error_unknown_channel(target_uuid.c_str());
}

void CreateCommentHandler::__handle406() const
{
	std::string target_uuid = _args.size() > 2 ? _args[2] : "";
	client_error_unknown_thread(target_uuid.c_str());
}

void CreateCommentHandler::__handle530() const { client_error_unauthorized(); }
