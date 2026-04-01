#include "client.hpp"
#include <iostream>
#include <poll.h>
#include <unistd.h>

Client::Client() : handler(tcp_client, context), is_connected(false) {}

void Client::displayPrompt() {
    std::cout << "[myteams]> ";
    std::cout.flush();
}

bool Client::handleUserInput(const std::string &input) {
    if (!parser.parse(input)) {
        std::cerr << "Error: Invalid command format" << std::endl;
        return true;
    }

    const std::string &cmd = parser.getCommand();
    const std::vector<std::string> &args = parser.getArgs();

    if (!handler.execute(cmd, args)) {
        std::cerr << "Error: Invalid command or arguments" << std::endl;
        return cmd != "logout";
    }
    return cmd != "logout";
}

bool Client::run(const std::string &ip, int port) {
    if (!tcp_client.connect(ip, port)) {
        std::cerr << "Error: Failed to connect to server" << std::endl;
        return false;
    }

    is_connected = true;
    std::cout << "Connected to " << ip << ":" << port << std::endl;
    std::cout << "Type /help for available commands" << std::endl;

    displayPrompt();

    struct pollfd fds[2];
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;
    fds[1].fd = tcp_client.getSocketFd();
    fds[1].events = POLLIN;

    std::string input_buffer;
    char buffer[256];

    while (is_connected) {
        int poll_result = poll(fds, 2, -1);

        if (poll_result < 0) {
            std::cerr << "Error: Poll failed" << std::endl;
            break;
        }

        if (fds[0].revents & POLLIN) {
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) { break; }

            input_buffer = buffer;
            if (!input_buffer.empty() && input_buffer.back() == '\n') { input_buffer.pop_back(); }

            if (input_buffer.empty()) {
                displayPrompt();
                continue;
            }

            if (!handleUserInput(input_buffer)) { is_connected = false; }

            displayPrompt();
        }

        if (fds[1].revents & POLLIN) {
            std::string response = tcp_client.receive();
            if (response.empty()) { break; }

            std::cout << response << std::endl;
            displayPrompt();
        }
    }

    disconnect();
    return true;
}

void Client::disconnect() {
    if (is_connected) {
        tcp_client.disconnect();
        is_connected = false;
        std::cout << "Disconnected from server" << std::endl;
    }
}
