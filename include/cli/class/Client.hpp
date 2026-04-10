#pragma once

#include <string>
#include <queue>
#include "class/TCPClient.hpp"
#include "class/CommandParser.hpp"
#include "class/Context.hpp"
#include "class/CommandHandler.hpp"
#include "class/ResponseHandler.hpp"

class Client {
private:
    TCPClient _tcp_client;
    CommandParser _parser;
    Context _context;
    CommandHandler _handler;
    ResponseHandler _response_handler;
    bool _is_connected;
    bool _waiting_for_response;
    bool _stdin_closed;
    bool _interactive_mode;
    std::queue<std::string> _command_queue;

    void __displayPrompt();
    void __processQueuedCommands();
    bool __queueUserInput(const std::string &input);
    bool __runInteractive();
    bool __runEcho();
    static bool __isFinalResponseCode(const std::string &response);

public:
    Client();
    bool run(const std::string &ip, int port);
    void disconnect();
};
