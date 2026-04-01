#pragma once

#include <string>
#include <vector>
#include "Context.hpp"
#include "TCPClient.hpp"

class CommandHandler {
private:
    TCPClient &tcp_client;
    Context &context;

    bool handleHelp();
    bool handleLogin(const std::vector<std::string> &args);
    bool handleLogout();
    bool handleUsers();
    bool handleUser(const std::vector<std::string> &args);
    bool handleSend(const std::vector<std::string> &args);
    bool handleMessages(const std::vector<std::string> &args);
    bool handleSubscribe(const std::vector<std::string> &args);
    bool handleUnsubscribe(const std::vector<std::string> &args);
    bool handleSubscribed(const std::vector<std::string> &args);
    bool handleUse(const std::vector<std::string> &args);
    bool handleCreate(const std::vector<std::string> &args);
    bool handleList();
    bool handleInfo();

    bool sendCommand(const std::string &command);

public:
    CommandHandler(TCPClient &client, Context &ctx);
    bool execute(const std::string &command, const std::vector<std::string> &args);
};
