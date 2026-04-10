#pragma once

#include <string>
#include <vector>

class CommandParser {
private:
    std::string _command;
    std::vector<std::string> _args;

    bool __parseQuotedString(const std::string &input, size_t &pos, std::string &result);
    bool __isValidArgument(const std::string &arg);

public:
    CommandParser();
    static std::string normalizeInput(std::string input);
    static std::vector<std::string> splitCommands(const std::string &input);
    bool parse(const std::string &input);
    const std::string &getCommand() const;
    const std::vector<std::string> &getArgs() const;
    int getArgCount() const;
};
