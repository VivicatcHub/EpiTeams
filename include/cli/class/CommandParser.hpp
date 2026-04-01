#pragma once

#include <string>
#include <vector>

class CommandParser {
private:
    std::string command;
    std::vector<std::string> args;

    bool parseQuotedString(const std::string &input, size_t &pos, std::string &result);
    bool isValidArgument(const std::string &arg);

public:
    CommandParser();
    bool parse(const std::string &input);
    const std::string &getCommand() const;
    const std::vector<std::string> &getArgs() const;
    int getArgCount() const;
};
