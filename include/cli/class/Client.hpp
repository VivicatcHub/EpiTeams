#pragma once

#include <string>
#include "class/TCPClient.hpp"
#include "class/CommandParser.hpp"
#include "class/Context.hpp"
#include "class/CommandHandler.hpp"

class Client {
private:
    TCPClient tcp_client;
    CommandParser parser;
    Context context;
    CommandHandler handler;
    bool is_connected;

    void displayPrompt();
    bool handleUserInput(const std::string &input);

public:
    Client();
    bool run(const std::string &ip, int port);
    void disconnect();
};
