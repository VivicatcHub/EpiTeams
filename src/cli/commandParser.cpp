#include "class/CommandParser.hpp"
#include <sstream>
#include <iostream>

CommandParser::CommandParser() {}

bool CommandParser::parseQuotedString(const std::string &input, size_t &pos, std::string &result) {
    if (pos >= input.length() || input[pos] != '"') { return false; }

    pos++;
    result.clear();

    while (pos < input.length() && input[pos] != '"') {
        result += input[pos];
        pos++;
    }

    if (pos >= input.length()) { return false; }

    pos++;
    return true;
}

bool CommandParser::isValidArgument(const std::string &arg) { return !arg.empty(); }

bool CommandParser::parse(const std::string &input) {
    if (input.empty()) { return false; }

    command.clear();
    args.clear();

    size_t pos = 0;
    while (pos < input.length() && input[pos] == ' ') { pos++; }

    if (pos >= input.length()) { return false; }

    if (input[pos] != '/') { return false; }

    pos++;
    size_t cmd_start = pos;

    while (pos < input.length() && input[pos] != ' ') { pos++; }

    command = input.substr(cmd_start, pos - cmd_start);

    while (pos < input.length()) {
        while (pos < input.length() && input[pos] == ' ') { pos++; }

        if (pos >= input.length()) { break; }

        if (input[pos] != '"') { return false; }

        std::string arg;
        if (!parseQuotedString(input, pos, arg)) { return false; }

        if (!isValidArgument(arg)) { return false; }

        args.push_back(arg);
    }

    return true;
}

const std::string &CommandParser::getCommand() const { return command; }

const std::vector<std::string> &CommandParser::getArgs() const { return args; }

int CommandParser::getArgCount() const { return args.size(); }
