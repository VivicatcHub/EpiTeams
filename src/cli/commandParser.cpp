#include "class/CommandParser.hpp"
#include <sstream>
#include <iostream>

CommandParser::CommandParser() {}

std::string CommandParser::normalizeInput(std::string input) {
    while (!input.empty() && input.back() == '\n') input.pop_back();
    return input;
}

std::vector<std::string> CommandParser::splitCommands(const std::string &input)
{
    std::vector<std::string> commands;
    std::string current;

    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] == '\n') {
            if (!current.empty()) {
                commands.push_back(current);
                current.clear();
            }
            continue;
        }
        current += input[i];
    }
    if (!current.empty()) commands.push_back(current);
    return commands;
}

bool CommandParser::__parseQuotedString(const std::string &input, size_t &pos, std::string &result) {
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

bool CommandParser::__isValidArgument(const std::string &arg) { return !arg.empty(); }

bool CommandParser::parse(const std::string &input) {
    if (input.empty()) { return false; }

    _command.clear();
    _args.clear();

    size_t pos = 0;
    while (pos < input.length() && input[pos] == ' ') { pos++; }

    if (pos >= input.length()) { return false; }

    if (input[pos] != '/') { return false; }

    pos++;
    size_t cmd_start = pos;

    while (pos < input.length() && input[pos] != ' ') { pos++; }

    _command = input.substr(cmd_start, pos - cmd_start);

    while (pos < input.length()) {
        while (pos < input.length() && input[pos] == ' ') { pos++; }

        if (pos >= input.length()) { break; }

        if (input[pos] != '"') { return false; }

        std::string arg;
        if (!__parseQuotedString(input, pos, arg)) { return false; }

        if (!__isValidArgument(arg)) { return false; }

        _args.push_back(arg);
    }

    return true;
}

const std::string &CommandParser::getCommand() const { return _command; }

const std::vector<std::string> &CommandParser::getArgs() const { return _args; }

int CommandParser::getArgCount() const { return _args.size(); }
