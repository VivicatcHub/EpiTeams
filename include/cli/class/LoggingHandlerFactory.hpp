#pragma once

#include "Context.hpp"
#include "class/CommandLoggingHandlers.hpp"
#include <memory>
#include <string>
#include <vector>

class LoggingHandlerFactory {
public:
    static std::unique_ptr<LoggingHandler> createHandler(const std::string &command, const std::vector<std::string> &args, const Context &context);

private:
    LoggingHandlerFactory() = delete;
    ~LoggingHandlerFactory() = delete;
};
