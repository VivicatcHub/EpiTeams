#pragma once

#include <string>
#include <vector>
#include "class/Context.hpp"

class ResponseHandler {
private:
    std::string _last_command;
    std::vector<std::string> _last_args;
    Context _context;
    bool _waiting_for_201;

    static std::string __cleanString(const std::string &value);
    static bool __extractCodeAndMessage(const std::string &line, std::string &code, std::string &message);
    void __processResponseCode(const std::string &code, const std::string &message);
    void __splitAndProcessLine(const std::string &line);
    void __messageReceipted(const std::string &message);

public:
    ResponseHandler();
    void setCommandContext(const std::string &command, const std::vector<std::string> &args, const Context &context);
    void handleServerResponse(const std::string &raw_response);
};
