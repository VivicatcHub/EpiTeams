#include "class/ResponseHandler.hpp"
#include "class/Context.hpp"
#include "class/LoggingHandlerFactory.hpp"
#include "class/MessageParser.hpp"
#include "logging_client.h"
#include <cctype>
#include <iostream>

ResponseHandler::ResponseHandler() : _waiting_for_202(false) {}

void ResponseHandler::setCommandContext(const std::string &command, const std::vector<std::string> &args, const Context &context)
{
    _last_command = command;
    _last_args = args;
    _context = context;
}

std::string ResponseHandler::__cleanString(const std::string &value)
{
    size_t start = 0;
    size_t end = value.size();

    while (start < value.size() && std::isspace(static_cast<unsigned char>(value[start]))) start++;
    while (end > start && std::isspace(static_cast<unsigned char>(value[end - 1]))) end--;
    return value.substr(start, end - start);
}

bool ResponseHandler::__extractCodeAndMessage(const std::string &line, std::string &code, std::string &message)
{
    std::string cleaned = __cleanString(line);

    if (cleaned.empty()) return false;
    if (cleaned.size() < 3 || !std::isdigit(static_cast<unsigned char>(cleaned[0])) || !std::isdigit(static_cast<unsigned char>(cleaned[1])) || !std::isdigit(static_cast<unsigned char>(cleaned[2]))) {
        code = "---";
        message = cleaned;
        return true;
    }

    code = cleaned.substr(0, 3);
    if (cleaned.size() == 3) {
        message.clear();
        return true;
    }
    if (cleaned[3] == ' ') {
        message = cleaned.substr(4);
        return true;
    }
    message = cleaned.substr(3);
    return true;
}

void ResponseHandler::__messageReceipted(const std::string &message)
{
    std::string msg, from;

    if (MessageParser::parseMessageReceipt(message, msg, from)) std::cout << "\n\n╭─ \033[36mmyteams\033[0m from \033[36m" << from << "\033[0m\n╰─$ " << msg.substr(1, msg.size() - 2) << "\n" << std::endl;
    else MessageParser::printResponseFailure(message);
}

void ResponseHandler::__processResponseCode(const std::string &code, const std::string &message)
{
    if (code == "225") {
        __messageReceipted(message);
        return;
    }
    if (code == "201" || code == "203") {
        _waiting_for_202 = true;
    } else if (_waiting_for_202 && code == "202") {
        _waiting_for_202 = false;
        return;
    }
    auto handler = LoggingHandlerFactory::createHandler(_last_command, _last_args);
    if (handler) {
        handler->handleResponse(code, message);
    } else {
        MessageParser::printResponsePrompt(code);
        std::cout << message << std::endl;
    }
}

void ResponseHandler::__splitAndProcessLine(const std::string &line)
{
    std::string code, message;

    __extractCodeAndMessage(line, code, message);
    __processResponseCode(code, message);
}

void ResponseHandler::handleServerResponse(const std::string &raw_response)
{
    std::string line;

    for (size_t i = 0; i <= raw_response.size(); i++) {
        if (i == raw_response.size() || raw_response[i] == '\n' || raw_response[i] == '\r') {
            if (!line.empty()) __splitAndProcessLine(line);
            line.clear();
            continue;
        }
        line += raw_response[i];
    }
}
