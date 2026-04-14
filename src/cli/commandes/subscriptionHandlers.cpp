/*
** EPITECH PROJECT, 2026
** MyTeams
** File description:
** Subscribe, Subscribed, and Unsubscribe logging handlers implementation
*/

#include "class/CommandLoggingHandlers.hpp"
#include "class/MessageParser.hpp"
#include "logging_client.h"
#include <iostream>

// ==================== SUBSCRIBE HANDLER ====================

void SubscribeHandler::handleResponse(const std::string &code,
	const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "200") __handle200();
	else if (code == "404") __handle404();
	else if (code == "530") __handle530();
	else std::cout << message << std::endl;
}

void SubscribeHandler::__handle200() const
{
	// Subscription successful - no specific logging function
}

void SubscribeHandler::__handle404() const
{
	// Team not found
	client_error_unknown_team("");
}

void SubscribeHandler::__handle530() const
{
	client_error_unauthorized();
}

// ==================== SUBSCRIBED HANDLER ====================

void SubscribedHandler::handleResponse(const std::string &code,
	const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "200") __handle200(message);
	else if (code == "201") __handle201(message);
	else if (code == "404") __handle404();
	else if (code == "530") __handle530();
	else std::cout << message << std::endl;
}

void SubscribedHandler::__handle200(const std::string &message) const
{
	// List of subscribed teams
	std::string team_name, team_uuid, team_description;

	if (MessageParser::parseTeamInfo(message, team_name, team_uuid, team_description)) {
		client_print_teams(team_uuid.c_str(), team_name.c_str(), team_description.c_str());
	}
}

void SubscribedHandler::__handle201(const std::string &message) const
{
	// List of team subscribers (users)
	std::string user_name, user_uuid;
	int user_status = 0;

	if (MessageParser::parseUserStatus(message, user_name, user_uuid, user_status)) {
		client_print_users(user_uuid.c_str(), user_name.c_str(), user_status);
	}
}

void SubscribedHandler::__handle404() const
{
	// Team not found
	client_error_unknown_team("");
}

void SubscribedHandler::__handle530() const
{
	client_error_unauthorized();
}

// ==================== UNSUBSCRIBE HANDLER ====================

void UnsubscribeHandler::handleResponse(const std::string &code,
	const std::string &message) const
{
	MessageParser::printResponsePrompt(code);

	if (code == "200") __handle200();
	else if (code == "404") __handle404();
	else if (code == "530") __handle530();
	else std::cout << message << std::endl;
}

void UnsubscribeHandler::__handle200() const
{
	// Unsubscription successful - no specific logging function
}

void UnsubscribeHandler::__handle404() const
{
	// Team not found
	client_error_unknown_team("");
}

void UnsubscribeHandler::__handle530() const
{
	client_error_unauthorized();
}
