#pragma once

#include <string>
#include <vector>
#include "Context.hpp"
#include "TCPClient.hpp"

class CommandHandler {
private:
    TCPClient &_tcp_client;
    Context &_context;

    bool __handleHelp();
    bool __handleLogin(const std::vector<std::string> &args);
    bool __handleLogout();
    bool __handleUsers();
    bool __handleUser(const std::vector<std::string> &args);
    bool __handleSend(const std::vector<std::string> &args);
    bool __handleMessages(const std::vector<std::string> &args);
    bool __handleSubscribe(const std::vector<std::string> &args);
    bool __handleUnsubscribe(const std::vector<std::string> &args);
    bool __handleSubscribed(const std::vector<std::string> &args);
    bool __handleUse(const std::vector<std::string> &args);
    bool __handleCreate(const std::vector<std::string> &args);
    bool __handleList();
    bool __handleInfo(const std::vector<std::string> &args);
    bool __sendCommand(const std::string &command);

public:
    CommandHandler(TCPClient &client, Context &ctx);
    bool execute(const std::string &command, const std::vector<std::string> &args);
};
