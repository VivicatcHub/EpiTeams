#pragma once

#include <string>
#include <vector>

class LoggingHandler {
public:
    virtual ~LoggingHandler() = default;
    virtual void handleResponse(const std::string &code, const std::string &message) const = 0;

protected:
    LoggingHandler() = default;
};
