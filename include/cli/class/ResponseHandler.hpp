#pragma once

#include <string>
#include <vector>

class ResponseHandler {
private:
    std::string _last_command;
    std::vector<std::string> _last_args;

    static std::string __cleanString(const std::string &value);
    static bool __extractCodeAndMessage(const std::string &line,
        std::string &code, std::string &message);
    static bool __parseUserStatusMessage(const std::string &message,
        std::string &user_name, std::string &user_uuid, int &user_status);

    void __printStyled(const std::string &code, const std::string &message) const;
    void __dispatchLogging(const std::string &code, const std::string &message) const;

public:
    ResponseHandler();
    void setCommandContext(const std::string &command,
        const std::vector<std::string> &args);
    void handleServerResponse(const std::string &raw_response) const;
    std::string getLastCommand() const;
    std::vector<std::string> getLastArgs() const;
};
