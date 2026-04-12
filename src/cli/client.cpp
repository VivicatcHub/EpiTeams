#include "client.hpp"
#include <cctype>
#include <iostream>
#include <poll.h>
#include <unistd.h>

Client::Client()
    : _handler(_tcp_client, _context),
    _is_connected(false),
    _waiting_for_response(false),
    _disconnect_after_response(false),
    _stdin_closed(false),
    _interactive_mode(false) {
}

bool Client::__isFinalResponseCode(const std::string &response)
{
    if (response.size() < 3
        || !std::isdigit(response[0])
        || !std::isdigit(response[1])
        || !std::isdigit(response[2])) return true;
    std::string code = response.substr(0, 3);
    if (code[0] == '2' || code[0] == '4' || code[0] == '5') return true;
    return false;
}

void Client::__handleUseResponse(const std::string &response)
{
    std::string first_line;
    size_t pos = 0;

    while (pos < response.size() && (response[pos] == '\r' || response[pos] == '\n'))
        pos++;
    size_t end = response.find_first_of("\r\n", pos);
    if (end == std::string::npos)
        first_line = response.substr(pos);
    else
        first_line = response.substr(pos, end - pos);

    std::string code;
    if (first_line.size() >= 3
        && std::isdigit(static_cast<unsigned char>(first_line[0]))
        && std::isdigit(static_cast<unsigned char>(first_line[1]))
        && std::isdigit(static_cast<unsigned char>(first_line[2]))) {
        code = first_line.substr(0, 3);
    }

    if (_response_handler.getLastCommand() == "use" && !code.empty() && code[0] == '2') {
        std::vector<std::string> last_args = _response_handler.getLastArgs();
        if (last_args.empty()) {
            _context.reset();
        } else {
            _context.reset();
            if (last_args.size() >= 1) _context.setTeamUuid(last_args[0]);
            if (last_args.size() >= 2) _context.setChannelUuid(last_args[1]);
            if (last_args.size() >= 3) _context.setThreadUuid(last_args[2]);
        }
    }
}

void Client::__displayPrompt() {
    std::cout << "\n╭─ \033[32mmyteams\033[0m\n╰─$ ";
    std::cout.flush();
}

bool Client::__queueUserInput(const std::string &input) {
    if (!_parser.parse(input)) {
        std::cerr << "Error: Invalid command format" << std::endl;
        return true;
    }

    const std::string &cmd = _parser.getCommand();
    if (cmd == "help") {
        if (!_handler.execute(cmd, _parser.getArgs())) std::cerr << "Error: Invalid command or arguments" << std::endl;
        return true;
    }

    _command_queue.push(input);
    return true;
}

void Client::__processQueuedCommands() {
    while (_is_connected && !_waiting_for_response && !_command_queue.empty()) {
        std::string input = _command_queue.front();
        _command_queue.pop();

        if (!_parser.parse(input)) {
            std::cerr << "Error: Invalid command format" << std::endl;
            continue;
        }

        const std::string &cmd = _parser.getCommand();
        const std::vector<std::string> &args = _parser.getArgs();
        _response_handler.setCommandContext(cmd, args);

        if (!_handler.execute(cmd, args)) {
            std::cerr << "Error: Invalid command or arguments" << std::endl;
            continue;
        }

        if (cmd == "use" && args.size() == 0) continue;
        _waiting_for_response = true;
        if (cmd == "logout") _disconnect_after_response = true;
    }

    if (_stdin_closed && _command_queue.empty() && !_waiting_for_response) _is_connected = false;
}

bool Client::__runEcho() {
    struct pollfd fds[2];

    fds[0].fd = -1;
    fds[0].events = POLLIN;
    fds[1].fd = _tcp_client.getSocketFd();
    fds[1].events = POLLIN;

    char buffer[256];

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        std::string input = CommandParser::normalizeInput(buffer);
        std::vector<std::string> commands = CommandParser::splitCommands(input);

        for (size_t i = 0; i < commands.size(); i++) {
            std::string command = CommandParser::normalizeInput(commands[i]);

            if (command.empty()) continue;
            if (!__queueUserInput(command)) {
                _is_connected = false;
                break;
            }
        }
        if (!_is_connected) break;
    }
    _stdin_closed = true;

    while (_is_connected) {
        int poll_result = poll(fds, 2, -1);

        if (poll_result < 0) {
            std::cerr << "Error: Poll failed" << std::endl;
            break;
        }

        if (fds[1].revents & POLLIN) {
            std::string response = _tcp_client.receive();
            if (response.empty()) {
                _is_connected = false;
                break;
            }
            _response_handler.handleServerResponse(response);

            if (_waiting_for_response && __isFinalResponseCode(response)) {
                __handleUseResponse(response);
                _waiting_for_response = false;
                if (_disconnect_after_response) _is_connected = false;
            }
        }
        __processQueuedCommands();
    }

    disconnect();
    return true;
}

bool Client::__runInteractive() {
    struct pollfd fds[2];
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;
    fds[1].fd = _tcp_client.getSocketFd();
    fds[1].events = POLLIN;

    char buffer[256];

    __displayPrompt();
    while (_is_connected) {
        int poll_result = poll(fds, 2, -1);

        if (poll_result < 0) {
            std::cerr << "Error: Poll failed" << std::endl;
            break;
        }

        if (fds[0].fd >= 0 && (fds[0].revents & POLLIN)) {
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                _stdin_closed = true;
                fds[0].fd = -1;
            } else {
                std::string input = CommandParser::normalizeInput(buffer);
                std::vector<std::string> commands = CommandParser::splitCommands(input);

                if (!commands.empty()) {
                    for (size_t i = 0; i < commands.size(); i++) {
                        std::string command = CommandParser::normalizeInput(commands[i]);

                        if (command.empty()) continue;
                        if (!__queueUserInput(command)) {
                            _is_connected = false;
                            break;
                        }
                    }
                } else if (!_waiting_for_response && _command_queue.empty()) __displayPrompt();
            }
        }

        if (fds[0].fd >= 0 && (fds[0].revents & (POLLHUP | POLLERR | POLLNVAL))) {
            _stdin_closed = true;
            fds[0].fd = -1;
        }

        if (fds[1].revents & POLLIN) {
            std::string response = _tcp_client.receive();
            if (response.empty()) {
                _is_connected = false;
                break;
            }
            _response_handler.handleServerResponse(response);

            if (_waiting_for_response && __isFinalResponseCode(response)) {
                __handleUseResponse(response);
                _waiting_for_response = false;
                if (_disconnect_after_response) _is_connected = false;
            }
        }
        __processQueuedCommands();
        if (_is_connected && !_waiting_for_response && _command_queue.empty()) __displayPrompt();
    }

    disconnect();
    return true;
}

bool Client::run(const std::string &ip, int port) {
    if (!_tcp_client.connect(ip, port)) {
        std::cerr << "Error: Failed to connect to server" << std::endl;
        return false;
    }

    _is_connected = true;
    _interactive_mode = isatty(STDIN_FILENO);
    std::cout << "Connected to " << ip << ":" << port << std::endl;
    std::cout << "Type /help for available commands" << std::endl;

    if (_interactive_mode) return __runInteractive();
    return __runEcho();
}

void Client::disconnect() {
    if (_is_connected) {
        _tcp_client.disconnect();
        _is_connected = false;
        std::cout << "Disconnected from server" << std::endl;
    }
}
