#include "class/TCPClient.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

TCPClient::TCPClient() : _socket_fd(-1), _port(-1) {}

TCPClient::~TCPClient() { disconnect(); }

bool TCPClient::connect(const std::string &ip, int port) {
    _socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket_fd < 0) { return false; }

    struct sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0) {
        close(_socket_fd);
        return false;
    }

    if (::connect(_socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(_socket_fd);
        _socket_fd = -1;
        return false;
    }

    this->_ip = ip;
    this->_port = port;
    return true;
}

bool TCPClient::send(const std::string &message) {
    if (_socket_fd < 0) { return false; }
    ssize_t bytes_sent = ::send(_socket_fd, message.c_str(), message.length(), 0);
    return bytes_sent > 0;
}

std::string TCPClient::receive() {
    if (_socket_fd < 0) { return ""; }
    char buffer[1024] = { 0 };
    ssize_t bytes_received = recv(_socket_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) { return ""; }
    return std::string(buffer, bytes_received);
}

bool TCPClient::isConnected() const { return _socket_fd >= 0; }

int TCPClient::getSocketFd() const { return _socket_fd; }

void TCPClient::disconnect() {
    if (_socket_fd >= 0) {
        close(_socket_fd);
        _socket_fd = -1;
    }
}
